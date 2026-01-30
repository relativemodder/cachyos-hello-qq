#include "commandrunner.h"
#include <QDebug>

CommandRunner::CommandRunner(QObject *parent)
    : QObject{parent},
    m_runningProcesses(0)
{}

void CommandRunner::incrementProcessCount() {
    m_runningProcesses++;
    emit runningCountChanged();
    if (m_runningProcesses == 1) {
        emit isRunningChanged();
    }
}

void CommandRunner::decrementProcessCount() {
    if (m_runningProcesses > 0) {
        m_runningProcesses--;
        emit runningCountChanged();
        if (m_runningProcesses == 0) {
            emit isRunningChanged();
        }
    }
}

void CommandRunner::runCommand(const QString &command) {
    QProcess *process = new QProcess(this);
    incrementProcessCount();

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                QString output = process->readAllStandardOutput();
                emit commandFinished(exitCode, output);
                decrementProcessCount();
                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred,
            this, [this, process](QProcess::ProcessError error) {
                emit commandError(process->errorString());
                decrementProcessCount();
                process->deleteLater();
            });

    process->start("/bin/sh", QStringList() << "-c" << command);
}

void CommandRunner::runCommandWithArgs(const QString &program, const QStringList &args) {
    QProcess *process = new QProcess(this);
    incrementProcessCount();

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {
                QString output = process->readAllStandardOutput();
                emit commandFinished(exitCode, output);
                decrementProcessCount();
                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred,
            this, [this, process](QProcess::ProcessError error) {
                emit commandError(process->errorString());
                decrementProcessCount();
                process->deleteLater();
            });

    process->start(program, args);
}
