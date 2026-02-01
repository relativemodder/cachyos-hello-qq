#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include <cstdlib>
#include "languageselectorbackend.h"
#include "commandrunner.h"
#include "cachyospirunner.h"
#include "cachyoskmrunner.h"
#include "fileloader.h"
#include "tweakcontroller.h"
#include "pacmanservice.h"
#include "networkingfixservice.h"
#include "autostartcontroller.h"
#include "installerbackend.h"

static QObject* installerBackendSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new InstallerBackend();
}

int main(int argc, char *argv[])
{
    if (getenv("DONT_FIX_60FPS") == NULL) {
        setenv("QSG_USE_SIMPLE_ANIMATION_DRIVER", "1", 1);
        setenv("QSG_RENDER_LOOP", "threaded", 1);
    }

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

    qmlRegisterSingletonType<InstallerBackend>(
        "org.cachyos.installer", 1, 0, "InstallerBackend",
        installerBackendSingletonProvider
        );

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.breeze"));
    }

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
