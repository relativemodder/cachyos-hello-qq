import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.Page {
    id: root
    title: qsTr("Install apps")

    signal back();
    signal next();

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Kirigami.Heading {
            text: qsTr("You can easily install new apps!")
        }

        Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("Install the most popular apps using our CachyOS Package Installer. Press a button below to open it!")
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
                icon.name: "plasmadiscover"
                icon.width: 150
                icon.height: 150
                text: qsTr("CachyOS Package Installer")
                display: AbstractButton.TextUnderIcon
                enabled: !piRunner?.isRunning
                onClicked: piRunner.runCommand("cachyos-pi");
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
