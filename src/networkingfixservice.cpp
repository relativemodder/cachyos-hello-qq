#include "networkingfixservice.h"

NetworkingFixService::NetworkingFixService(TerminalLauncher* terminalLauncher,
                                           QObject *parent)
    : QObject(parent)
    , m_terminalLauncher(terminalLauncher)
    , m_currentOperation(RankMirrors)
{
    connect(m_terminalLauncher, &TerminalLauncher::finished,
            this, &NetworkingFixService::onTerminalFinished);
    connect(m_terminalLauncher, &TerminalLauncher::failed,
            this, &NetworkingFixService::onTerminalFailed);
}

void NetworkingFixService::rankMirrors()
{
    m_currentOperation = RankMirrors;
    emit operationStarted(tr("Ranking mirrors"));
    QString cmd = buildRankMirrorsCommand();
    m_terminalLauncher->launch(cmd);
}

QString NetworkingFixService::buildRankMirrorsCommand()
{
    return "pkexec cachyos-rate-mirrors";
}

void NetworkingFixService::changeDNS(const QString& dnsProvider)
{
    m_currentOperation = ChangeDNS;
    emit operationStarted(tr("Changing DNS server"));
    QString cmd = buildChangeDNSCommand(dnsProvider);
    m_terminalLauncher->launch(cmd);
}

QString NetworkingFixService::buildChangeDNSCommand(const QString& provider)
{
    QString dns1, dns2;

    if (provider == "cloudflare") {
        dns1 = "1.1.1.1";
        dns2 = "1.0.0.1";
    } else if (provider == "google") {
        dns1 = "8.8.8.8";
        dns2 = "8.8.4.4";
    } else {
        // default to cloudflare
        dns1 = "1.1.1.1";
        dns2 = "1.0.0.1";
    }

    return QString("pkexec sh -c '"
                   "echo \"nameserver %1\" > /etc/resolv.conf && "
                   "echo \"nameserver %2\" >> /etc/resolv.conf && "
                   "echo \"DNS changed to %3\" && "
                   "sleep 2"
                   "'").arg(dns1, dns2, provider);
}

void NetworkingFixService::onTerminalFinished(int exitCode)
{
    bool success = (exitCode == 0);
    QString message;

    switch (m_currentOperation) {
    case RankMirrors:
        message = success
                      ? tr("Mirrors ranked successfully")
                      : tr("Mirror ranking failed or cancelled");
        break;
    case ChangeDNS:
        message = success
                      ? tr("DNS server changed successfully")
                      : tr("DNS change failed or cancelled");
        break;
    default:
        message = success ? tr("Operation completed") : tr("Operation failed");
        break;
    }

    emit operationFinished(success, message);
}

void NetworkingFixService::onTerminalFailed(int errorCode, const QString& errorMessage)
{
    emit operationFinished(false,
                           QString(tr("Terminal error (%1): %2"))
                               .arg(errorCode)
                               .arg(errorMessage));
}
