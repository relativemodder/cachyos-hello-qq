#include "autostartcontroller.h"
#include <QFileInfo>
#include <QDir>

AutostartController::AutostartController(QObject *parent)
    : QObject{parent}
{
}

bool AutostartController::isAutostart() const {
    QFileInfo autostartFile(QDir::homePath() + "/.config/autostart/org.cachyos.hello.desktop");
    return autostartFile.exists();
}

void AutostartController::setAutostart(bool enabled) {
    QString autostartPath = QDir::homePath() + "/.config/autostart";
    QString autostartFile = autostartPath + "/org.cachyos.hello.desktop";

    if (enabled) {
        QDir().mkpath(autostartPath);
        QFile::copy("/usr/share/applications/org.cachyos.hello.desktop", autostartFile);
    } else {
        QFile::remove(autostartFile);
    }
}
