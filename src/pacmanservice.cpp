#include "pacmanservice.h"
#include <QProcess>
#include <QDebug>

PacmanService::PacmanService(CommandRunner* commandRunner,
                             TerminalLauncher* terminalLauncher,
                             QObject *parent)
    : QObject(parent)
    , m_commandRunner(commandRunner)
    , m_terminalLauncher(terminalLauncher)
    , m_currentOperation(PackageInstall)
    , m_usingTerminal(false)
{
    // CommandRunner (nonint)
    connect(m_commandRunner, &CommandRunner::commandFinished,
            this, &PacmanService::onCommandFinished);
    connect(m_commandRunner, &CommandRunner::commandError,
            this, &PacmanService::onCommandError);

    // TerminalLauncher (interactive)
    connect(m_terminalLauncher, &TerminalLauncher::finished,
            this, &PacmanService::onTerminalFinished);
    connect(m_terminalLauncher, &TerminalLauncher::failed,
            this, &PacmanService::onTerminalFailed);
}

bool PacmanService::isPackageInstalledSync(const QString& packageName)
{
    QProcess process;
    process.start("pacman", QStringList() << "-Qi" << packageName);
    process.waitForFinished();

    return process.exitCode() == 0;
}

void PacmanService::installPackage(const QString& packageName)
{
    installPackages(QStringList() << packageName);
}

void PacmanService::installPackages(const QStringList& packageNames)
{
    m_currentOperation = PackageInstall;
    m_currentPackageName = packageNames.join(" ");
    m_usingTerminal = true;

    QStringList toInstall;
    for (const QString& pkg : packageNames) {
        if (!isPackageInstalledSync(pkg)) {
            toInstall << pkg;
        }
    }

    if (toInstall.isEmpty()) {
        emit operationFinished(true, tr("All packages are already installed"));
        return;
    }

    emit operationStarted(tr("Installing packages"));

    QString cmd = buildInstallCommand(packageNames);
    m_terminalLauncher->launch(cmd);
}

QString PacmanService::buildInstallCommand(const QStringList& packages)
{
    QString pkgList = packages.join(" ");
    return QString("pkexec pacman -S --needed %1").arg(pkgList);
}

void PacmanService::updateSystem()
{
    m_currentOperation = SystemUpdate;
    m_usingTerminal = true;

    emit operationStarted(tr("System update"));

    QString cmd = buildUpdateCommand();
    m_terminalLauncher->launch(cmd);
}

QString PacmanService::buildUpdateCommand()
{
    return "pkexec pacman -Syu";
}

void PacmanService::reinstallAllPackages()
{
    m_currentOperation = ReinstallAll;
    m_usingTerminal = true;

    emit operationStarted(tr("Reinstalling all packages"));

    QString cmd = buildReinstallCommand();
    m_terminalLauncher->launch(cmd);
}

QString PacmanService::buildReinstallCommand()
{
    return "pkexec pacman -S $(pacman -Qnq)";
}

void PacmanService::removeOrphans()
{
    m_currentOperation = RemoveOrphans;
    m_usingTerminal = true;

    emit operationStarted(tr("Removing orphan packages"));

    QString cmd = buildRemoveOrphansCommand();
    m_terminalLauncher->launch(cmd);
}

QString PacmanService::buildRemoveOrphansCommand()
{
    return "orphans=$(pacman -Qtdq); "
           "if [ -n \"$orphans\" ]; then "
           "  echo 'WARNING! ONLY DO IT IF YOU KNOW WHAT YOU ARE DOING!'; "
           "  pkexec pacman -Rns $orphans; "
           "else "
           "  echo 'No orphan packages found.'; "
           "  sleep 2; "
           "fi";
}

void PacmanService::resetKeyrings()
{
    m_currentOperation = ResetKeyring;
    m_usingTerminal = true;

    emit operationStarted(tr("Resetting keyrings"));

    QString cmd = buildResetKeyringCommand();
    m_terminalLauncher->launch(cmd);
}

QString PacmanService::buildResetKeyringCommand()
{
    return "pkexec sh -c 'rm -rf /etc/pacman.d/gnupg && "
           "pacman-key --init && pacman-key --populate && "
           "pacman-key --recv-keys F3B607488DB35A47 --keyserver keyserver.ubuntu.com && "
           "pacman-key --lsign-key F3B607488DB35A47'";
}

void PacmanService::clearCache()
{
    m_currentOperation = ClearCache;
    m_usingTerminal = false;

    emit operationStarted(tr("Clearing package cache"));

    QString cmd = "pkexec sh -c 'paccache -rk2 2>/dev/null || pkexec rm -rf /var/cache/pacman/pkg/*'";
    m_commandRunner->runCommand(cmd);
}

void PacmanService::removeDatabaseLock()
{
    m_currentOperation = RemoveLock;
    m_usingTerminal = false;

    emit operationStarted(tr("Removing database lock"));

    QString cmd = "pkexec rm -f /var/lib/pacman/db.lck";
    m_commandRunner->runCommand(cmd);
}


void PacmanService::onCommandFinished(int exitCode, const QString& output)
{
    if (m_usingTerminal) {
        return;
    }

    bool success = (exitCode == 0);

    QString message;
    switch (m_currentOperation) {
    case ClearCache:
        message = success ? tr("Package cache cleared") : tr("Failed to clear cache");
        break;
    case RemoveLock:
        message = success ? tr("Database lock removed") : tr("Failed to remove lock");
        break;
    default:
        if (!success) {
            message = tr("Operation failed");
        }
        break;
    }

    emit operationFinished(success, message);
}

void PacmanService::onCommandError(const QString& error)
{
    if (m_usingTerminal) {
        return;
    }

    emit operationFinished(false, error);
}

void PacmanService::onTerminalFinished(int exitCode)
{
    if (!m_usingTerminal) {
        return;
    }

    m_usingTerminal = false;
    bool success = (exitCode == 0);

    QString message;
    switch (m_currentOperation) {
    case PackageInstall:
        message = success
                      ? QString(tr("Package(s) %1 installed successfully")).arg(m_currentPackageName)
                      : tr("Installation failed or cancelled");
        break;

    case SystemUpdate:
        message = success ? tr("System updated successfully") : tr("Update failed or cancelled");
        break;

    case ReinstallAll:
        message = success ? tr("All packages reinstalled") : tr("Reinstall failed or cancelled");
        break;

    case RemoveOrphans:
        message = success ? tr("Orphan packages removed") : tr("Remove orphans failed or cancelled");
        break;

    case ResetKeyring:
        message = success ? tr("Keyrings reset successfully") : tr("Keyring reset failed");
        break;

    default:
        message = success ? tr("Operation completed") : tr("Operation failed");
        break;
    }

    emit operationFinished(success, message);
}

void PacmanService::onTerminalFailed(int errorCode, const QString& errorMessage)
{
    if (!m_usingTerminal) {
        return;
    }

    m_usingTerminal = false;
    emit operationFinished(false, QString(tr("Terminal error (%1): %2")).arg(errorCode).arg(errorMessage));
}
