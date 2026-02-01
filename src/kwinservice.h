#ifndef KWINSERVICE_H
#define KWINSERVICE_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>

class KWinService : public QObject
{
    Q_OBJECT

public:
    explicit KWinService(QObject *parent = nullptr);
    ~KWinService();

    Q_INVOKABLE void showDebugConsole();
    Q_INVOKABLE bool isKWinRunning();

signals:
    void debugConsoleOpened();
    void operationFailed(const QString& error);

private slots:
    void onDBusCallFinished(QDBusPendingCallWatcher* watcher);

private:
    QDBusInterface* m_kwinInterface;

    void ensureInterface();
};

#endif // KWINSERVICE_H
