#ifndef TWEAKREPOSITORY_H
#define TWEAKREPOSITORY_H

#include "tweakdefinition.h"
#include <QHash>
#include <QObject>

class TweakRepository : public QObject
{
    Q_OBJECT

public:
    static TweakRepository& instance();

    TweakDefinition getTweak(const QString& id) const;
    QList<TweakDefinition> getAllTweaks() const;
    bool exists(const QString& id) const;

private:
    TweakRepository();
    void initializeTweaks();

    QHash<QString, TweakDefinition> m_tweaks;
};

#endif // TWEAKREPOSITORY_H
