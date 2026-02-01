#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include "languageselectorbackend.h"
#include "commandrunner.h"
#include "cachyospirunner.h"
#include "cachyoskmrunner.h"
#include "fileloader.h"
#include "tweakcontroller.h"
#include "pacmanservice.h"
#include "networkingfixservice.h"
#include "autostartcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/qt/qml/CachyOsHello/img/icon.png"));
    app.setDesktopFileName("org.cachyos.hello.desktop");

    qmlRegisterType<TweakController>("org.cachyos.tweaks", 1, 0, "TweakController");
    qmlRegisterUncreatableType<PacmanService>("org.cachyos.tweaks", 1, 0, "PacmanService",
                                              "Use through TweakController");
    qmlRegisterUncreatableType<KWinService>("org.cachyos.tweaks", 1, 0, "KWinService",
                                            "Use through TweakController");
    qmlRegisterUncreatableType<NetworkingFixService>("org.cachyos.tweaks", 1, 0, "NetworkingFixService",
                                            "Use through NetworkingFixService");

    qmlRegisterType<AutostartController>("org.cachyos.autostart", 1, 0, "AutostartController");

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
