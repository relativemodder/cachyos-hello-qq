#include "kwinservice.h"
#include <QDBusConnection>
#include <QDBusReply>
#include <QDBusPendingReply>
#include <QDebug>

KWinService::KWinService(QObject *parent)
    : QObject(parent)
    , m_kwinInterface(nullptr)
{
    ensureInterface();
}

KWinService::~KWinService()
{
    delete m_kwinInterface;
}

void KWinService::ensureInterface()
{
    if (m_kwinInterface) {
        return;
    }

    m_kwinInterface = new QDBusInterface(
        "org.kde.KWin",
        "/KWin",
        "org.kde.KWin",
        QDBusConnection::sessionBus(),
        this
    );

    if (!m_kwinInterface->isValid()) {
        qWarning() << "Failed to connect to KWin DBus interface:"
                   << m_kwinInterface->lastError().message();
        delete m_kwinInterface;
        m_kwinInterface = nullptr;
    }
}

bool KWinService::isKWinRunning()
{
    ensureInterface();
    return m_kwinInterface && m_kwinInterface->isValid();
}

void KWinService::showDebugConsole()
{
    if (!isKWinRunning()) {
        emit operationFailed(tr("KWin is not running or DBus interface is unavailable"));
        return;
    }

    QDBusPendingCall call = m_kwinInterface->asyncCall("showDebugConsole");
    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, &KWinService::onDBusCallFinished);
}

void KWinService::onDBusCallFinished(QDBusPendingCallWatcher* watcher)
{
    QDBusPendingReply<> reply = *watcher;

    if (reply.isError()) {
        QString error = reply.error().message();
        qWarning() << "KWin DBus call failed:" << error;
        emit operationFailed(error);
    } else {
        emit debugConsoleOpened();
    }

    watcher->deleteLater();
}
