#include "cachyoskmrunner.h"

CachyosKmRunner::CachyosKmRunner(QObject *parent)
    : CommandRunner{parent}
{}

void CachyosKmRunner::run() {
    runCommand("cachyos-kernel-manager");
}
