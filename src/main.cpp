#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "languageselectorbackend.h"
#include "commandrunner.h"
#include "cachyospirunner.h"
#include "cachyoskmrunner.h"
#include "fileloader.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    LanguageSelectorBackend lsb(&engine, &app);
    CommandRunner cmdRunner(&app);
    CachyosPiRunner piRunner(&app);
    CachyosKmRunner kmRunner(&app);
    FileLoader fileLoader(&app);

    auto rootContext = engine.rootContext();

    rootContext->setContextProperty("lsb", &lsb);
    rootContext->setContextProperty("cmdRunner", &cmdRunner);
    rootContext->setContextProperty("piRunner", &piRunner);
    rootContext->setContextProperty("kmRunner", &kmRunner);
    rootContext->setContextProperty("fileLoader", &fileLoader);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("CachyOsHello", "Main");

    return app.exec();
}
