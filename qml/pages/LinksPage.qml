import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.cachyos.installer
import "../components"

Kirigami.Page {
    id: root
    title: qsTr("Links")

    signal back();
    signal done();
    signal tweaks();
    signal installer();

    Component {
        id: groupBoxBackground
        Rectangle {
            color: "transparent"
            border.width: 1
            Kirigami.Theme.colorSet: Kirigami.Theme.Header
            border.color: Kirigami.Theme.disabledTextColor
            radius: Kirigami.Units.cornerRadius
            opacity: 0.5
        }
    }

    ReadmeDialog {
        id: readmeDialog
    }

    ReleaseInfoDialog {
        id: releaseInfoDialog
    }

    GetInvolvedDialog {
        id: getInvolvedDialog
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Kirigami.Units.largeSpacing

        Kirigami.Heading {
            text: qsTr("Any questions?")
        }

        Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("You can find the answers to your questions down here:")
        }

        GroupBox {
            Layout.minimumWidth: 0
            Layout.fillWidth: true
            Layout.preferredWidth: 0
            Layout.topMargin: Kirigami.Units.largeSpacing

            background: Loader {
                sourceComponent: groupBoxBackground
            }

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: qsTr("Documentation")
                    padding: Kirigami.Units.largeSpacing
                    font.weight: 600
                    opacity: 0.6
                }

                Kirigami.ActionToolBar {
                    Layout.fillWidth: true

                    actions: [
                        Kirigami.Action {
                            text: qsTr("Read me")
                            icon.name: "documentation"
                            onTriggered: readmeDialog.open()
                        },
                        Kirigami.Action {
                            text: qsTr("Release info")
                            icon.name: "info"
                            onTriggered: releaseInfoDialog.open()
                        },
                        Kirigami.Action {
                            text: qsTr("Wiki")
                            icon.name: "insert-endnote"
                            onTriggered: Qt.openUrlExternally("https://wiki.cachyos.org/")
                        }
                    ]
                }
            }
        }

        GroupBox {
            Layout.minimumWidth: 0
            Layout.fillWidth: true
            Layout.preferredWidth: 0
            Layout.topMargin: Kirigami.Units.largeSpacing

            background: Loader {
                sourceComponent: groupBoxBackground
            }

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: qsTr("Support")
                    padding: Kirigami.Units.largeSpacing
                    font.weight: 600
                    opacity: 0.6
                }

                Kirigami.ActionToolBar {
                    Layout.fillWidth: true

                    actions: [
                        Kirigami.Action {
                            text: qsTr("Forum")
                            icon.name: "dialog-messages"
                            onTriggered: Qt.openUrlExternally("https://discuss.cachyos.org/")
                        },
                        Kirigami.Action {
                            text: qsTr("Software")
                            icon.name: "search"
                            onTriggered: Qt.openUrlExternally("https://packages.cachyos.org/")
                        }
                    ]
                }
            }
        }


        GroupBox {
            Layout.minimumWidth: 0
            Layout.fillWidth: true
            Layout.preferredWidth: 0
            Layout.topMargin: Kirigami.Units.largeSpacing

            background: Loader {
                sourceComponent: groupBoxBackground
            }

            ColumnLayout {
                anchors.fill: parent

                Label {
                    text: qsTr("Project")
                    padding: Kirigami.Units.largeSpacing
                    font.weight: 600
                    opacity: 0.6
                }

                Kirigami.ActionToolBar {
                    Layout.fillWidth: true

                    actions: [
                        Kirigami.Action {
                            text: qsTr("Get involved")
                            icon.name: "gnumeric-brush"
                            onTriggered: getInvolvedDialog.open()
                        },
                        Kirigami.Action {
                            text: qsTr("Development")
                            icon.name: "format-text-code"
                            onTriggered: Qt.openUrlExternally("https://github.com/cachyos")
                        },
                        Kirigami.Action {
                            text: qsTr("Donate")
                            icon.name: "donate"
                            onTriggered: Qt.openUrlExternally("https://www.patreon.com/CachyOS")
                        },
                        Kirigami.Action {
                            text: "Reddit"
                            icon.source: "qrc:/qt/qml/CachyOsHello/img/reddit.svg"
                            icon.color: "white"
                            onTriggered: Qt.openUrlExternally("https://www.reddit.com/r/cachyos/")
                        },
                        Kirigami.Action {
                            text: "Discord"
                            icon.name: "qrc:/qt/qml/CachyOsHello/img/discord.svg"
                            onTriggered: Qt.openUrlExternally("https://discord.gg/cachyos-862292009423470592")
                        }
                    ]
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Button {
                leftPadding: 20
                rightPadding: 25
                icon.name: "tools-wizard"
                icon.height: 12
                text: InstallerBackend.inIso ? qsTr("Launch installer") : qsTr("Go to tweaks and fixes")
                onClicked: InstallerBackend.inIso ? root.installer() : root.tweaks()
                highlighted: InstallerBackend.inIso
            }

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
                icon.name: "checkmark"
                icon.height: 12
                text: qsTr("Done")
                highlighted: !InstallerBackend.inIso
                onClicked: root.done()
            }
        }
    }
}
