#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

#include <QObject>
#include <QProcess>

class CommandRunner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
    Q_PROPERTY(int runningCount READ runningCount NOTIFY runningCountChanged)

public:
    explicit CommandRunner(QObject *parent = nullptr);

    Q_INVOKABLE void runCommand(const QString &command);
    Q_INVOKABLE void runCommandWithArgs(const QString &program, const QStringList &args);

    bool isRunning() const { return m_runningProcesses > 0; }
    int runningCount() const { return m_runningProcesses; }

signals:
    void commandFinished(int exitCode, const QString &output);
    void commandError(const QString &error);
    void isRunningChanged();
    void runningCountChanged();

private:
    void incrementProcessCount();
    void decrementProcessCount();

    int m_runningProcesses;
};

#endif // COMMANDRUNNER_H
