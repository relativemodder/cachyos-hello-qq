#include "installerbackend.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFileInfo>
#include <QThread>
#include <QDebug>

extern "C" {
int check_handheld_support();
}

InstallerBackend::InstallerBackend(QObject* parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

QString InstallerBackend::readFileToString(const QString& path, const QString& defaultValue) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return defaultValue;
    }

    QTextStream in(&file);
    QString content = in.readAll().trimmed();
    file.close();
    return content;
}

// bool InstallerBackend::checkRegularFile(const QString& path) {
//     QFileInfo fileInfo(path);
//     return fileInfo.exists() && fileInfo.isFile();
// }

bool InstallerBackend::outdatedVersionCheck(const QString& message) {
    QString editionTag = readFileToString("/etc/edition-tag", "desktop");
    QString versionTag = readFileToString("/etc/version-tag", "testing");

    if (versionTag.contains("testing")) {
        qDebug() << "InstallerBackend::outdatedVersionCheck testing";
        emit showMessage(static_cast<int>(MessageType::Warning),
                         tr("Testing ISO Warning"),
                         tr("You are using a testing ISO. This version may contain bugs and is not recommended for production use.")
        );
        return true;
    }

    QNetworkRequest request(QUrl("https://cachyos.org/versions.json"));
    QNetworkReply* reply = m_networkManager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        emit showMessage(static_cast<int>(MessageType::Warning),
                         tr("Offline Error"),
                         tr("You're offline. Can't install the system.")
                        );
        reply->deleteLater();
        return false;
    }

    QByteArray responseData = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse versions.json";
        return true;
    }

    QJsonObject versionsObj = jsonDoc.object();
    QString latestVersion = editionTag.contains("desktop")
                                ? versionsObj["desktopISOVersion"].toString()
                                : versionsObj["handheldISOVersion"].toString();
    latestVersion = latestVersion.trimmed();

    bool versionOk, latestOk;
    uint32_t parsedVersion = versionTag.toUInt(&versionOk);
    uint32_t parsedLatest = latestVersion.toUInt(&latestOk);

    if (versionOk && latestOk && parsedVersion > parsedLatest) {
        qDebug() << "versionOk && latestOk && parsedVersion > parsedLatest";
        emit showMessage(static_cast<int>(MessageType::Warning),
                         tr("Testing ISO Warning"),
                         tr("You are using a testing ISO. This version may contain bugs and is not recommended for production use.")
                    );
        return true;
    }

    if (versionTag != latestVersion) {
        emit showMessage(static_cast<int>(MessageType::Warning),
                         tr("Outdated Version Warning"),
                         tr("You are using an outdated ISO. Download the latest one from CachyOS website.")
                        );
    }

    return true;
}

bool InstallerBackend::editionCompatCheck(const QString& message) {
    QString editionTag = readFileToString("/etc/edition-tag", "desktop");

    if (editionTag == "handheld") {
        int result = check_handheld_support();

        if (result != 0) {
            emit showMessage(static_cast<int>(MessageType::Warning),
                tr("Handheld is not supported"),
                tr("You're trying to install CachyOS on unsupported hardware.")
            );
            return false;
        }
    }

    return true;
}

bool InstallerBackend::connectivityCheck(const QString& message) {
    QNetworkRequest request(QUrl("https://cachyos.org"));
    QNetworkReply* reply = m_networkManager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool httpSuccess = (reply->error() == QNetworkReply::NoError);
    reply->deleteLater();

    if (httpSuccess) {
        return true;
    }

    QStringList targets = {
        "2001:4860:4860::8888", "2606:4700:4700::1111", "2620:fe::fe",
        "8.8.8.8", "1.1.1.1", "9.9.9.9"
    };

    for (const QString& target : targets) {
        QProcess ping;
        ping.start("/sbin/ping", {"-c", "1", "-W", "3", target});
        ping.waitForFinished(5000);

        if (ping.exitCode() == 0) {
            qInfo() << "yeah, we have connection (pinged" << target << ")";
            return true;
        }
    }

    emit showMessage(static_cast<int>(MessageType::Error),
                     tr("Offline Error"),
                     tr("You're offline. Can't install the system.")
                    );
    return false;
}

void InstallerBackend::launchInstaller(const QString& message) {
    if (m_installing) {
        return;
    }

    QThread* workerThread = QThread::create([this, message]() {
        setInstalling(true);

        if (!connectivityCheck(message)) {
            qInfo() << "connectivity check failed";
            setInstalling(false);
            return;
        }

        if (!editionCompatCheck(message)) {
            qInfo() << "edition compatibility check failed";
            setInstalling(false);
            return;
        }

        if (!outdatedVersionCheck(message)) {
            qInfo() << "version check failed";
            setInstalling(false);
            return;
        }

        qInfo() << "iso checks passed. starting installer...";

        QProcess installer;
        installer.setProcessChannelMode(QProcess::MergedChannels);
        installer.start("/usr/local/bin/calamares-online.sh");

        connect(&installer, &QProcess::readyReadStandardOutput, [&installer]() {
            QByteArray output = installer.readAllStandardOutput();
            qInfo().noquote() << output.trimmed();
        });

        installer.waitForFinished(-1);
        int exitCode = installer.exitCode();

        qInfo() << "installer finished with exit code:" << exitCode;

        emit installerFinished(exitCode);
        setInstalling(false);
    });

    workerThread->start();
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
}

bool InstallerBackend::isIso() {
    return QFile::exists("/run/archiso");
}

void InstallerBackend::setInstalling(bool installing) {
    if (m_installing != installing) {
        m_installing = installing;
        emit installingChanged();
    }
}
