#include "systemdservice.h"
#include <QProcess>

SystemdService::SystemdService(QObject *parent)
    : QObject(parent)
{
}

bool SystemdService::checkUnitStatus(const QString& unit, bool isUser)
{
    QProcess process;
    QStringList args = {"is-active", unit};

    if (isUser) {
        args.prepend("--user");
    }

    process.start("systemctl", args);
    process.waitForFinished(3000);

    QString output = process.readAllStandardOutput().trimmed();
    return output == "active";
}

bool SystemdService::isSystemUnitActive(const QString& unit)
{
    if (m_systemCache.contains(unit)) {
        return m_systemCache[unit];
    }

    bool active = checkUnitStatus(unit, false);
    m_systemCache[unit] = active;
    return active;
}

bool SystemdService::isUserUnitActive(const QString& unit)
{
    if (m_userCache.contains(unit)) {
        return m_userCache[unit];
    }

    bool active = checkUnitStatus(unit, true);
    m_userCache[unit] = active;
    return active;
}

bool SystemdService::areSystemUnitsActive(const QStringList& units)
{
    for (const QString& unit : units) {
        if (!isSystemUnitActive(unit)) {
            return false;
        }
    }
    return !units.isEmpty();
}

bool SystemdService::areUserUnitsActive(const QStringList& units)
{
    for (const QString& unit : units) {
        if (!isUserUnitActive(unit)) {
            return false;
        }
    }
    return !units.isEmpty();
}

void SystemdService::refreshSystemCache()
{
    m_systemCache.clear();
}

void SystemdService::refreshUserCache()
{
    m_userCache.clear();
}
