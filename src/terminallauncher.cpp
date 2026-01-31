#include "terminallauncher.h"
#include <QStandardPaths>
#include <QTextStream>
#include <QFile>
#include <QDBusInterface>
#include <QDBusConnection>

TerminalLauncher::TerminalLauncher(QObject *parent)
    : QObject(parent)
    , m_shell("bash")
{
    initializeTerminals();
}

void TerminalLauncher::setShell(const QString& shell) {
    m_shell = shell;
}

void TerminalLauncher::launch(const QString& command) {
    // find available terminal
    QString terminal = findTerminal();
    if (terminal.isEmpty()) {
        sendNotification("No terminal installed",
                         "Could not find a terminal emulator. Please install one.");
        emit failed(1, "No terminal emulator found");
        return;
    }

    // create temporary file
    m_tempFile.reset(new QTemporaryFile());
    m_tempFile->setAutoRemove(false);

    if (!m_tempFile->open()) {
        emit failed(1, "Failed to create temporary file");
        return;
    }

    QString scriptContent = command;

    // special handling for kgx
    if (terminal == "kgx") {
        scriptContent += "\nkill -SIGQUIT $PPID 2>/dev/null";
    }

    QTextStream out(m_tempFile.get());
    out << scriptContent;
    m_tempFile->close();

    // build and execute command
    QString termCmd = buildCommand(terminal);
    executeTerminal(termCmd, terminal);
}

void TerminalLauncher::onProcessStarted() {
    emit started();
}

void TerminalLauncher::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    cleanup();

    if (exitStatus == QProcess::CrashExit) {
        emit failed(2, "Process crashed");
    } else {
        emit finished(exitCode);
    }
}

void TerminalLauncher::onProcessError(QProcess::ProcessError error) {
    QString errorMsg;
    switch (error) {
    case QProcess::FailedToStart:
        errorMsg = "Failed to start terminal";
        break;
    case QProcess::Crashed:
        errorMsg = "Terminal crashed";
        break;
    case QProcess::Timedout:
        errorMsg = "Process timeout";
        break;
    default:
        errorMsg = "Unknown error";
    }

    cleanup();
    emit failed(2, errorMsg);
}

void TerminalLauncher::onReadyReadStandardOutput() {
    if (m_process) {
        QString output = QString::fromLocal8Bit(m_process->readAllStandardOutput());
        emit outputReady(output);
    }
}

void TerminalLauncher::onReadyReadStandardError() {
    if (m_process) {
        QString error = QString::fromLocal8Bit(m_process->readAllStandardError());
        emit errorOutputReady(error);
    }
}

void TerminalLauncher::initializeTerminals() {
    m_terminals = {
        {"alacritty", "alacritty -e %1"},
        {"foot", "foot %1"},
        {"ghostty", "ghostty -e %1"},
        {"kgx", "kgx --wait -e %1"},
        {"gnome-terminal", "gnome-terminal --wait -- %1"},
        {"kitty", "kitty %1"},
        {"konsole", "konsole -e %1"},
        {"lxterminal", "lxterminal -e %1"},
        {"rio", "rio -e %1"},
        {"st", "st %1"},
        {"terminator", "terminator -x %1"},
        {"xfce4-terminal", "xfce4-terminal --disable-server --command '%1'"},
        {"xterm", "xterm -e %1"}
    };

    m_termOrder = {
        "alacritty", "rio", "ghostty", "kitty", "konsole",
        "kgx", "gnome-terminal", "xfce4-terminal", "lxterminal",
        "xterm", "st", "foot", "terminator"
    };
}

QString TerminalLauncher::findTerminal() {
    QString envTerminal = qEnvironmentVariable("TERMINAL");
    if (!envTerminal.isEmpty() && !QStandardPaths::findExecutable(envTerminal).isEmpty()) {
        return envTerminal;
    }

    for (const QString& term : m_termOrder) {
        if (!QStandardPaths::findExecutable(term).isEmpty()) {
            return term;
        }
    }

    return QString();
}

QString TerminalLauncher::buildCommand(const QString& terminal) {
    QString cmd = m_shell + " " + m_tempFile->fileName();

    if (terminal == "alacritty") {
        return QString("alacritty -e %1 || LIBGL_ALWAYS_SOFTWARE=1 alacritty -e %1")
        .arg(cmd);
    }

    return m_terminals[terminal].arg(cmd);
}

void TerminalLauncher::executeTerminal(const QString& termCmd, const QString& terminal) {
    m_currentTerminal = terminal;
    m_process.reset(new QProcess(this));

    // connect signals
    connect(m_process.get(), &QProcess::started,
            this, &TerminalLauncher::onProcessStarted);
    connect(m_process.get(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &TerminalLauncher::onProcessFinished);
    connect(m_process.get(), &QProcess::errorOccurred,
            this, &TerminalLauncher::onProcessError);
    connect(m_process.get(), &QProcess::readyReadStandardOutput,
            this, &TerminalLauncher::onReadyReadStandardOutput);
    connect(m_process.get(), &QProcess::readyReadStandardError,
            this, &TerminalLauncher::onReadyReadStandardError);

    m_process->setProgram("/bin/sh");
    m_process->setArguments({"-c", termCmd});
    m_process->start();
}

void TerminalLauncher::cleanup() {
    if (m_tempFile) {
        QFile::remove(m_tempFile->fileName());
        m_tempFile.reset();
    }
}

void TerminalLauncher::sendNotification(const QString& title, const QString& message) {
    QDBusInterface interface("org.freedesktop.Notifications",
                             "/org/freedesktop/Notifications",
                             "org.freedesktop.Notifications",
                             QDBusConnection::sessionBus());

    if (interface.isValid()) {
        interface.call("Notify", "CachyOS", uint(0), QString(),
                       title, message, QStringList(), QVariantMap(), 1500);
    }
}
