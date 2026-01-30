#ifndef CACHYOSKMRUNNER_H
#define CACHYOSKMRUNNER_H

#include <QObject>
#include "commandrunner.h"

class CachyosKmRunner : public CommandRunner
{
    Q_OBJECT
public:
    explicit CachyosKmRunner(QObject *parent = nullptr);
    Q_INVOKABLE void run();
};

#endif // CACHYOSKMRUNNER_H
