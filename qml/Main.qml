import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "./pages"
import org.cachyos.installer

Kirigami.ApplicationWindow {
    id: root
    width: 797
    height: 592

    minimumWidth: 797
    minimumHeight: 592

    visible: true
    title: qsTr("CachyOS Hello")

    pageStack.defaultColumnWidth: width

    Kirigami.Dialog {
        id: messageDialog

        property int messageType: 0

        title: {
            switch (messageType) {
                case 2: return qsTr("Error")
                case 1: return qsTr("Warning")
                default: return qsTr("Information")
            }
        }

        padding: Kirigami.Units.largeSpacing
        standardButtons: Kirigami.Dialog.Ok

        Label {
            id: messageLabel
            Layout.fillWidth: true
            Layout.preferredWidth: Kirigami.Units.gridUnit * 20
            wrapMode: Text.Wrap
        }
    }

    Kirigami.InlineMessage {
        id: inlineMessage
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: Kirigami.Units.largeSpacing
            topMargin: Kirigami.Units.largeSpacing * 7
        }

        showCloseButton: true
        z: 999

        Timer {
            id: hideTimer
            interval: 5000
            onTriggered: inlineMessage.visible = false
        }
    }

    function showNotification(messageType, title, message, useDialog = false) {
        messageDialog.messageType = messageType
        messageLabel.text = message

        if (useDialog || messageType === 2) {
            messageDialog.open()
        } else {
            inlineMessage.type = messageType === 1
                ? Kirigami.MessageType.Warning
                : Kirigami.MessageType.Information
            inlineMessage.text = title + ": " + message
            inlineMessage.visible = true
            hideTimer.restart()
        }
    }

    function startInstaller() {
        InstallerBackend.launchInstaller(qsTr("Starting installation..."))
    }

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
            onNext: root.pageStack.push(linksPageComponent)
        }
    }

    Component {
        id: tweaksPageComponent
        TweaksAndFixesPage {
            onClose: root.pageStack.layers.pop()
        }
    }

    Component {
        id: linksPageComponent
        LinksPage {
            onBack: root.pageStack.goBack()
            onDone: Qt.quit()
            onTweaks: root.pageStack.layers.push(tweaksPageComponent)
            onInstaller: root.startInstaller()
        }
    }

    Component.onCompleted: {
        InstallerBackend.showMessage.connect((messageType, title, message) => {
            showNotification(messageType, title, message)
        })

        InstallerBackend.installerFinished.connect((exitCode) => {
            showNotification(
                exitCode === 0 ? 0 : 2,
                exitCode === 0 ? qsTr("Success") : qsTr("Error"),
                exitCode === 0
                    ? qsTr("Installation completed successfully")
                    : qsTr("Installation failed with code: ") + exitCode,
                true
            )
        })
    }

    pageStack.initialPage: MainPage {
        onNext: root.pageStack.push(InstallerBackend.inIso ? linksPageComponent : installAppsPageComponent)
        onTweaks: root.pageStack.layers.push(tweaksPageComponent)
        onInstaller: root.startInstaller()
    }

    footer: Item {
        height: 0

        PageIndicator {
            currentIndex: root.pageStack.currentIndex
            count: InstallerBackend.inIso ? 2 : 4
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -20
        }
    }
}
