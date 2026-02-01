#include "tweakcontroller.h"
#include "tweakrepository.h"
#include "pacmanservice.h"
#include <QDebug>

TweakController::TweakController(QObject *parent)
    : QObject(parent)
    , m_commandRunner(new CommandRunner(this))
    , m_terminalLauncher(new TerminalLauncher(this))
    , m_systemdService(new SystemdService(this))
    , m_pacmanService(new PacmanService(new CommandRunner(this), new TerminalLauncher(this), this))
    , m_kwinService(new KWinService(this))
    , m_networkingFixService(new NetworkingFixService(new TerminalLauncher(this), this))
    , m_currentTargetState(false)
    , m_currentTweakType(TweakDefinition::SystemService)
    , m_waitingForPackageInstall(false)
{
    connect(m_commandRunner, &CommandRunner::commandFinished,
            this, &TweakController::onCommandFinished);
    connect(m_commandRunner, &CommandRunner::commandError,
            this, &TweakController::onCommandError);
    connect(m_pacmanService, &PacmanService::operationFinished,
            this, &TweakController::onPackageInstalled);
}

bool TweakController::isTweakEnabled(const QString& tweakId)
{
    auto& repo = TweakRepository::instance();
    if (!repo.exists(tweakId)) {
        return false;
    }

    TweakDefinition tweak = repo.getTweak(tweakId);
    return isTweakCurrentlyEnabled(tweak);
}

bool TweakController::isTweakCurrentlyEnabled(const TweakDefinition& tweak)
{
    switch (tweak.type) {
    case TweakDefinition::SystemService:
        return m_systemdService->areSystemUnitsActive(tweak.units);
    case TweakDefinition::UserService:
        return m_systemdService->areUserUnitsActive(tweak.units);
    case TweakDefinition::Sysctl:
        return false;
    }
    return false;
}

void TweakController::toggleTweak(const QString& tweakId)
{
    auto& repo = TweakRepository::instance();
    if (!repo.exists(tweakId)) {
        emit operationFinished(tweakId, false, tr("Unknown tweak"));
        return;
    }

    TweakDefinition tweak = repo.getTweak(tweakId);
    m_currentTweakId = tweakId;
    m_currentTweakType = tweak.type;

    if (!checkAndInstallPackage(tweak)) {
        return;
    }

    bool currentState = isTweakCurrentlyEnabled(tweak);
    performToggle(tweak, currentState);
}

bool TweakController::checkAndInstallPackage(const TweakDefinition& tweak)
{
    if (tweak.packageName.isEmpty()) {
        return true;
    }

    if (m_pacmanService->isPackageInstalledSync(tweak.packageName)) {
        return true;
    }

    emit packageNotInstalled(m_currentTweakId, tweak.packageName);

    m_waitingForPackageInstall = true;
    m_pacmanService->installPackage(tweak.packageName);

    return false;
}

void TweakController::onPackageInstalled(bool success, const QString& message)
{
    if (!m_waitingForPackageInstall) {
        return;
    }

    m_waitingForPackageInstall = false;

    if (!success) {
        emit tweakStateNeedsRefresh(m_currentTweakId);
        emit operationFinished(m_currentTweakId, false, tr("Package installation failed or cancelled"));
        return;
    }

    auto& repo = TweakRepository::instance();
    TweakDefinition tweak = repo.getTweak(m_currentTweakId);
    bool currentState = isTweakCurrentlyEnabled(tweak);
    performToggle(tweak, currentState);
}

void TweakController::performToggle(const TweakDefinition& tweak, bool currentState)
{
    m_currentTargetState = !currentState;

    emit operationStarted(m_currentTweakId);

    QString cmd = buildToggleCommand(tweak, m_currentTargetState);
    m_commandRunner->runCommand(cmd);
}

QString TweakController::buildToggleCommand(const TweakDefinition& tweak, bool enable)
{
    QString action = enable ? "enable --now" : "disable --now";
    QString userFlag = (tweak.type == TweakDefinition::UserService) ? "--user " : "";
    QString pkexecPrefix = (tweak.type == TweakDefinition::SystemService) ? "pkexec " : "";

    QStringList commands;
    for (const QString& unit : tweak.units) {
        commands << QString("%1systemctl %2%3 %4")
        .arg(pkexecPrefix)
            .arg(userFlag)
            .arg(action)
            .arg(unit);
    }

    return commands.join(" && ");
}

void TweakController::refreshCacheForTweak(const TweakDefinition& tweak)
{
    if (tweak.type == TweakDefinition::SystemService) {
        m_systemdService->refreshSystemCache();
    } else if (tweak.type == TweakDefinition::UserService) {
        m_systemdService->refreshUserCache();
    }
}

void TweakController::onCommandFinished(int exitCode, const QString& output)
{
    if (m_waitingForPackageInstall) {
        return;
    }

    auto& repo = TweakRepository::instance();
    TweakDefinition tweak = repo.getTweak(m_currentTweakId);

    refreshCacheForTweak(tweak);

    bool success = (exitCode == 0);

    if (success) {
        emit tweakStateChanged(m_currentTweakId, m_currentTargetState);
        emit operationFinished(m_currentTweakId, true, tr("Success"));
    } else {
        emit tweakStateNeedsRefresh(m_currentTweakId);

        bool actualState = isTweakCurrentlyEnabled(tweak);
        emit tweakStateChanged(m_currentTweakId, actualState);

        emit operationFinished(m_currentTweakId, false, output);
    }
}

void TweakController::onCommandError(const QString& error)
{
    if (m_waitingForPackageInstall) {
        return;
    }

    auto& repo = TweakRepository::instance();
    TweakDefinition tweak = repo.getTweak(m_currentTweakId);

    if (m_currentTweakType == TweakDefinition::SystemService) {
        m_systemdService->refreshSystemCache();
    } else if (m_currentTweakType == TweakDefinition::UserService) {
        m_systemdService->refreshUserCache();
    }

    emit tweakStateNeedsRefresh(m_currentTweakId);

    bool actualState = isTweakCurrentlyEnabled(tweak);
    emit tweakStateChanged(m_currentTweakId, actualState);

    emit operationFinished(m_currentTweakId, false, error);
}

QString TweakController::getTweakName(const QString& tweakId) const
{
    auto& repo = TweakRepository::instance();
    return repo.getTweak(tweakId).name;
}

QString TweakController::getTweakDescription(const QString& tweakId) const
{
    auto& repo = TweakRepository::instance();
    return repo.getTweak(tweakId).description;
}

