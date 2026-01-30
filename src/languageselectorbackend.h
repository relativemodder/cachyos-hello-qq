#ifndef LANGUAGESELECTORBACKEND_H
#define LANGUAGESELECTORBACKEND_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>

class LanguageSelectorBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentLanguage READ currentLanguage NOTIFY currentLanguageChanged)

public:
    explicit LanguageSelectorBackend(QQmlApplicationEngine* engine, QObject *parent = nullptr);

    Q_INVOKABLE void switchLanguage(const QString &lang);
    QString currentLanguage() const { return m_currentLanguage; }

signals:
    void currentLanguageChanged();

private:
    void loadInitialLanguage();

    QQmlApplicationEngine* m_engine;
    QTranslator m_translator;
    QString m_currentLanguage;
    QString m_translationPrefix;
    QString m_translationPath;
};

#endif // LANGUAGESELECTORBACKEND_H
