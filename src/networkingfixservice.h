#ifndef NETWORKINGFIXSERVICE_H
#define NETWORKINGFIXSERVICE_H

#include <QObject>
#include "terminallauncher.h"

class NetworkingFixService : public QObject
{
    Q_OBJECT

public:
    enum Operation {
        RankMirrors,
        ChangeDNS
    };

    explicit NetworkingFixService(TerminalLauncher* terminalLauncher,
                                  QObject *parent = nullptr);

    Q_INVOKABLE void rankMirrors();
    Q_INVOKABLE void changeDNS(const QString& dnsProvider = "cloudflare");

signals:
    void operationStarted(const QString& operation);
    void operationFinished(bool success, const QString& message);

private slots:
    void onTerminalFinished(int exitCode);
    void onTerminalFailed(int errorCode, const QString& errorMessage);

private:
    QString buildRankMirrorsCommand();
    QString buildChangeDNSCommand(const QString& provider);

    TerminalLauncher* m_terminalLauncher;
    Operation m_currentOperation;
};

#endif // NETWORKINGFIXSERVICE_H
