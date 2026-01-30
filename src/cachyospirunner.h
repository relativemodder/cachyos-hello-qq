#ifndef CACHYOSPIRUNNER_H
#define CACHYOSPIRUNNER_H

#include <QObject>
#include "commandrunner.h"

class CachyosPiRunner : public CommandRunner
{
    Q_OBJECT
public:
    explicit CachyosPiRunner(QObject *parent = nullptr);
    Q_INVOKABLE void run();
};

#endif // CACHYOSPIRUNNER_H
