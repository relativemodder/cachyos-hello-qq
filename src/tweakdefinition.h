#ifndef TWEAKDEFINITION_H
#define TWEAKDEFINITION_H

#include <QString>
#include <QStringList>

struct TweakDefinition {
    QString id;
    QString name;
    QString description;

    enum Type {
        SystemService,
        UserService,
        Sysctl
    } type;

    QStringList units;
    QString packageName;
};

#endif // TWEAKDEFINITION_H
