import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.Page {
    id: root
    title: qsTr("Manage kernels")

    signal back();
    signal next();

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Kirigami.Heading {
            text: qsTr("You can easily manage the Linux kernel settings!")
        }

        Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("There're a lot of kernels in the CachyOS Kernel Manager. Choose one, no, two, no, how many you want and rock with 'em!")
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Item {
                Layout.fillWidth: true
            }
            Button {
                id: cachyOsPiButton
                icon.name: "org.cachyos.KernelManager"
                icon.width: 100
                icon.height: 100
                Layout.minimumHeight: 180
                text: qsTr("CachyOS Kernel Manager")
                display: AbstractButton.TextUnderIcon
                enabled: !kmRunner?.isRunning
                onClicked: kmRunner.runCommand("cachyos-kernel-manager");
            }
            Item {
                Layout.fillWidth: true
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Button {
                leftPadding: 20
                rightPadding: 25
                icon.name: "go-previous-view-page"
                icon.height: 12
                text: qsTr("Back")
                onClicked: root.back()
            }

            Button {
                leftPadding: 20
                rightPadding: 25
                icon.name: "go-next-view-page"
                icon.height: 12
                text: qsTr("Next")
                onClicked: root.next()
            }
        }
    }
}
