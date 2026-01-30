#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "languageselectorbackend.h"
#include "commandrunner.h"
#include "cachyospirunner.h"
#include "cachyoskmrunner.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    LanguageSelectorBackend lsb(&engine, &app);
    CommandRunner cmdRunner(&app);
    CachyosPiRunner piRunner(&app);
    CachyosKmRunner kmRunner(&app);

    engine.rootContext()->setContextProperty("lsb", &lsb);
    engine.rootContext()->setContextProperty("cmdRunner", &cmdRunner);
    engine.rootContext()->setContextProperty("piRunner", &piRunner);
    engine.rootContext()->setContextProperty("kmRunner", &kmRunner);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CachyOsHello", "Main");

    return app.exec();
}
