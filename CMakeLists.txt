cmake_minimum_required(VERSION 3.16)

project(CachyOsHello VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick LinguistTools Widgets Core QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appCachyOsHello
    src/main.cpp
)

qt_add_translations(appCachyOsHello
    TS_FILES
        translations/CachyOsHello_ru.ts
)

qt_add_qml_module(appCachyOsHello
    URI CachyOsHello
    QML_FILES
        qml/Main.qml
        qml/pages/MainPage.qml
        qml/pages/TestPage.qml
        qml/pages/InstallAppsPage.qml
    RESOURCES
        img/icon.png
        img/cachyos-pi.png
    SOURCES src/languageselectorbackend.h src/languageselectorbackend.cpp
    SOURCES src/commandrunner.h src/commandrunner.cpp
    SOURCES src/cachyospirunner.h src/cachyospirunner.cpp
    QML_FILES qml/pages/KernelManagerPage.qml
    SOURCES src/cachyoskmrunner.h src/cachyoskmrunner.cpp
    QML_FILES qml/pages/TweaksAndFixesPage.qml
)

target_link_libraries(appCachyOsHello
    PRIVATE Qt6::Quick Qt6::Widgets Qt6::Core Qt6::QuickControls2
)

target_include_directories(appCachyOsHello PUBLIC src)

include(GNUInstallDirs)
install(TARGETS appCachyOsHello
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
