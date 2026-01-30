import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.desktop as Breeze

Kirigami.Page {
    id: root
    title: qsTr("Tweaks and fixes")

    signal close();

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Kirigami.Heading {
            text: qsTr("Tweak the system and apply various fixes as you see fit!")
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Breeze.Switch {
                    text: qsTr("Use RAM for browser profiles")
                    topPadding: 5
                    bottomPadding: 5
                }

                Breeze.Switch {
                    text: qsTr("Auto-adjust process priorities for better system responsiveness")
                    topPadding: 5
                    bottomPadding: 5
                }

                Breeze.Switch {
                    text: qsTr("Enable CachyOS update notifier in tray")
                    topPadding: 5
                    bottomPadding: 5
                }

                Breeze.Switch {
                    text: qsTr("Proactively kill processes during low memory to prevent freezes")
                    topPadding: 5
                    bottomPadding: 5
                }

                Breeze.Switch {
                    text: qsTr("Automatically tune system network using Bfptune")
                    topPadding: 5
                    bottomPadding: 5
                }

                Breeze.Switch {
                    text: qsTr("Enable Bluetooth support")
                    topPadding: 5
                    bottomPadding: 5
                }
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
                icon.name: "dialog-close"
                icon.height: 12
                text: qsTr("Close")
                onClicked: root.close()
            }
        }
    }
}
