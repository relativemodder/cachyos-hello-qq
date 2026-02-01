#ifndef TWEAKCONTROLLER_H
#define TWEAKCONTROLLER_H

#include <QObject>
#include <QHash>
#include "commandrunner.h"
#include "systemdservice.h"
#include "pacmanservice.h"
#include "tweakdefinition.h"
#include "kwinservice.h"
#include "networkingfixservice.h"

class TweakController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CommandRunner* commandRunner READ commandRunner CONSTANT)
    Q_PROPERTY(PacmanService* pacmanService READ pacmanService CONSTANT)
    Q_PROPERTY(KWinService* kwinService READ kwinService CONSTANT)
    Q_PROPERTY(NetworkingFixService* networkingFixService READ networkingFixService CONSTANT)

public:
    explicit TweakController(QObject *parent = nullptr);

    Q_INVOKABLE bool isTweakEnabled(const QString& tweakId);
    Q_INVOKABLE void toggleTweak(const QString& tweakId);

    Q_INVOKABLE QString getTweakName(const QString& tweakId) const;
    Q_INVOKABLE QString getTweakDescription(const QString& tweakId) const;

    CommandRunner* commandRunner() const { return m_commandRunner; }
    PacmanService* pacmanService() const { return m_pacmanService; }
    KWinService* kwinService() const { return m_kwinService; }
    NetworkingFixService* networkingFixService() const { return m_networkingFixService; }

signals:
    void tweakStateChanged(const QString& tweakId, bool enabled);
    void operationStarted(const QString& tweakId);
    void operationFinished(const QString& tweakId, bool success, const QString& message);
    void packageNotInstalled(const QString& tweakId, const QString& packageName);
    void tweakStateNeedsRefresh(const QString& tweakId);

private slots:
    void onCommandFinished(int exitCode, const QString& output);
    void onCommandError(const QString& error);
    void onPackageInstalled(bool success, const QString& message);

private:
    CommandRunner* m_commandRunner;
    TerminalLauncher* m_terminalLauncher;
    SystemdService* m_systemdService;
    PacmanService* m_pacmanService;
    KWinService* m_kwinService;
    NetworkingFixService* m_networkingFixService;

    QString m_currentTweakId;
    bool m_currentTargetState;
    TweakDefinition::Type m_currentTweakType;
    bool m_waitingForPackageInstall;

    bool isTweakCurrentlyEnabled(const TweakDefinition& tweak);
    void performToggle(const TweakDefinition& tweak, bool currentState);
    bool checkAndInstallPackage(const TweakDefinition& tweak);

    QString buildToggleCommand(const TweakDefinition& tweak, bool enable);
    void refreshCacheForTweak(const TweakDefinition& tweak);
};


#endif // TWEAKCONTROLLER_H
