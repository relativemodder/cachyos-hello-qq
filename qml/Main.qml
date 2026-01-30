import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "./pages"

Kirigami.ApplicationWindow {
    id: root
    width: 700
    height: 550

    minimumWidth: 560
    minimumHeight: 500

    visible: true
    title: qsTr("CachyOS Hello")

    pageStack.defaultColumnWidth: width

    Component {
        id: installAppsPageComponent
        InstallAppsPage {
            onBack: root.pageStack.goBack()
            onNext: root.pageStack.push(kernelManagerPageComponent)
        }
    }

    Component {
        id: kernelManagerPageComponent
        KernelManagerPage {
            onBack: root.pageStack.goBack()
        }
    }

    Component {
        id: tweaksPageComponent
        TweaksAndFixesPage {
            onClose: {
                root.pageStack.layers.pop()
            }
        }
    }

    pageStack.initialPage: MainPage {
        onNext: root.pageStack.push(installAppsPageComponent)
        onTweaks: root.pageStack.layers.push(tweaksPageComponent)
    }

    footer: Item {
        height: 0

        PageIndicator {
            currentIndex: root.pageStack.currentIndex
            count: 3
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -20
        }
    }
}
