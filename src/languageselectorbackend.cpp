#include "languageselectorbackend.h"
#include <QGuiApplication>
#include <QLocale>
#include <QDebug>

LanguageSelectorBackend::LanguageSelectorBackend(QQmlApplicationEngine* engine, QObject *parent)
    : QObject{parent},
    m_engine(engine),
    m_translationPrefix("CachyOsHello"),
    m_translationPath(":/i18n")
{
    loadInitialLanguage();
}

void LanguageSelectorBackend::loadInitialLanguage() {
    QLocale systemLocale = QLocale::system();

    if (m_translator.load(systemLocale, m_translationPrefix, "_", m_translationPath)) {
        qApp->installTranslator(&m_translator);
        m_currentLanguage = systemLocale.name();
        qDebug() << "Loaded initial translation:" << m_currentLanguage;
    } else {
        // english fallback
        m_currentLanguage = "en_US";
        qDebug() << "Using default language:" << m_currentLanguage;
    }

    emit currentLanguageChanged();
}

void LanguageSelectorBackend::switchLanguage(const QString &lang) {
    if (m_currentLanguage == lang) {
        return; // language is installed already
    }

    qApp->removeTranslator(&m_translator);

    QLocale locale(lang);
    if (m_translator.load(locale, m_translationPrefix, "_", m_translationPath)) {
        qApp->installTranslator(&m_translator);
        m_currentLanguage = locale.name();
        qDebug() << "Switched to language:" << m_currentLanguage;
        emit currentLanguageChanged();
    } else {
        if (lang != "en") {
            qWarning() << "Failed to load translation for" << lang;
        }
        qApp->installTranslator(&m_translator);
    }

    m_engine->retranslate();
}
