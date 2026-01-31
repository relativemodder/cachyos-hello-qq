#ifndef TERMINALLAUNCHER_H
#define TERMINALLAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QTemporaryFile>
#include <QMap>
#include <QStringList>
#include <QScopedPointer>

class TerminalLauncher : public QObject {
    Q_OBJECT

public:
    explicit TerminalLauncher(QObject *parent = nullptr);

    void setShell(const QString& shell);

public slots:
    void launch(const QString& command);

signals:
    void started();
    void finished(int exitCode);
    void failed(int errorCode, const QString& errorMessage);
    void outputReady(const QString& output);
    void errorOutputReady(const QString& error);

private slots:
    void onProcessStarted();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();

private:
    void initializeTerminals();
    QString findTerminal();
    QString buildCommand(const QString& terminal);
    void executeTerminal(const QString& termCmd, const QString& terminal);
    void cleanup();
    void sendNotification(const QString& title, const QString& message);

    QString m_shell;
    QString m_currentTerminal;
    QMap<QString, QString> m_terminals;
    QStringList m_termOrder;
    QScopedPointer<QProcess> m_process;
    QScopedPointer<QTemporaryFile> m_tempFile;
};

#endif // TERMINALLAUNCHER_H
