#ifndef PACMANSERVICE_H
#define PACMANSERVICE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "commandrunner.h"
#include "terminallauncher.h"

class PacmanService : public QObject
{
    Q_OBJECT

public:
    explicit PacmanService(CommandRunner* commandRunner,
                           TerminalLauncher* terminalLauncher,
                           QObject *parent = nullptr);

    Q_INVOKABLE bool isPackageInstalledSync(const QString& packageName);

    // packages (interactive)
    Q_INVOKABLE void installPackage(const QString& packageName);
    Q_INVOKABLE void installPackages(const QStringList& packageNames);

    // system (interactive)
    Q_INVOKABLE void updateSystem();
    Q_INVOKABLE void reinstallAllPackages();
    Q_INVOKABLE void removeOrphans();
    Q_INVOKABLE void resetKeyrings();

    // fixes (nonint)
    Q_INVOKABLE void clearCache();
    Q_INVOKABLE void removeDatabaseLock();

signals:
    void packageCheckFinished(const QString& packageName, bool installed);
    void operationStarted(const QString& operation);
    void operationFinished(bool success, const QString& message);
    void installationRequired(const QString& packageName);

private slots:
    void onCommandFinished(int exitCode, const QString& output);
    void onCommandError(const QString& error);
    void onTerminalFinished(int exitCode);
    void onTerminalFailed(int errorCode, const QString& errorMessage);

private:
    CommandRunner* m_commandRunner;
    TerminalLauncher* m_terminalLauncher;

    enum OperationType {
        PackageInstall,
        SystemUpdate,
        ReinstallAll,
        RemoveOrphans,
        ClearCache,
        RemoveLock,
        ResetKeyring
    };

    OperationType m_currentOperation;
    QString m_currentPackageName;
    bool m_usingTerminal;

    QString buildInstallCommand(const QStringList& packages);
    QString buildUpdateCommand();
    QString buildReinstallCommand();
    QString buildRemoveOrphansCommand();
    QString buildResetKeyringCommand();
};

#endif // PACMANSERVICE_H
