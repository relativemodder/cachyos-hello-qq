#ifndef SYSTEMDSERVICE_H
#define SYSTEMDSERVICE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

class SystemdService : public QObject
{
    Q_OBJECT

public:
    explicit SystemdService(QObject *parent = nullptr);

    bool isSystemUnitActive(const QString& unit);
    bool isUserUnitActive(const QString& unit);

    bool areSystemUnitsActive(const QStringList& units);
    bool areUserUnitsActive(const QStringList& units);

    void refreshSystemCache();
    void refreshUserCache();

private:
    bool checkUnitStatus(const QString& unit, bool isUser);

    QHash<QString, bool> m_systemCache;
    QHash<QString, bool> m_userCache;
};

#endif // SYSTEMDSERVICE_H
