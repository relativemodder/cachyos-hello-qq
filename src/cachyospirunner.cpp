#include "cachyospirunner.h"

CachyosPiRunner::CachyosPiRunner(QObject *parent)
    : CommandRunner{parent}
{}

void CachyosPiRunner::run() {
    runCommand("cachyos-pi");
}
