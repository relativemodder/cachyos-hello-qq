#ifndef INSTALLERBACKEND_H
#define INSTALLERBACKEND_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QProcess>

class InstallerBackend : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool installing READ isInstalling NOTIFY installingChanged)
    Q_PROPERTY(bool inIso READ isIso)

public:
    explicit InstallerBackend(QObject* parent = nullptr);
    ~InstallerBackend() override = default;

    bool isInstalling() const { return m_installing; }

    Q_INVOKABLE void launchInstaller(const QString& message);
    Q_INVOKABLE bool isIso();

signals:
    void installingChanged();
    void showMessage(int messageType, const QString& title, const QString& message);
    void installerFinished(int exitCode);

private:
    bool outdatedVersionCheck(const QString& message);
    bool editionCompatCheck(const QString& message);
    bool connectivityCheck(const QString& message);

    QString readFileToString(const QString& path, const QString& defaultValue = QString());
    // bool checkRegularFile(const QString& path);

    void setInstalling(bool installing);

    QNetworkAccessManager* m_networkManager;
    bool m_installing{false};
};

enum class MessageType {
    Info = 0,
    Warning = 1,
    Error = 2
};

#endif // INSTALLERBACKEND_H
