import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "../components"

Kirigami.Page {
    id: root
    title: qsTr("Tweaks and fixes")

    signal close();

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

    ColumnLayout {
        anchors.fill: parent

        ScrollView {
            Layout.fillWidth: true
            contentWidth: availableWidth
            Layout.fillHeight: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

            ColumnLayout {
                anchors.fill: parent
                spacing: Kirigami.Units.largeSpacing

                Kirigami.Heading {
                    text: qsTr("System tweaks")
                }

                WrapSwitch {
                    text: qsTr("Use RAM for browser profiles")
                }

                WrapSwitch {
                    text: qsTr("Auto-adjust process priorities for better system responsiveness")
                }

                WrapSwitch {
                    text: qsTr("Enable CachyOS update notifier in tray")
                }

                WrapSwitch {
                    text: qsTr("Proactively kill processes during low memory to prevent freezes")
                }

                WrapSwitch {
                    text: qsTr("Automatically tune system network using Bfptune")
                }

                WrapSwitch {
                    text: qsTr("Enable Bluetooth support")
                }

                Kirigami.Heading {
                    text: qsTr("Fixes")
                    Layout.topMargin: Kirigami.Units.largeSpacing * 1.5
                }

                GroupBox {
                    Layout.minimumWidth: 0
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0

                    background: Loader {
                        sourceComponent: groupBoxBackground
                    }

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            text: qsTr("Package management")
                            padding: Kirigami.Units.largeSpacing
                            font.weight: 600
                            opacity: 0.6
                        }

                        Kirigami.ActionToolBar {
                            Layout.fillWidth: true

                            actions: [
                                Kirigami.Action {
                                    text: qsTr("System update")
                                    icon.name: "system-reboot-update"
                                },
                                Kirigami.Action {
                                    text: qsTr("Reinstall all packages")
                                    icon.name: "system-reboot"
                                },
                                Kirigami.Action {
                                    text: qsTr("Reset keyrings")
                                    icon.name: "edit-undo"
                                }
                            ]
                        }

                        Kirigami.ActionToolBar {
                            Layout.fillWidth: true


                            actions: [
                                Kirigami.Action {
                                    text: qsTr("Remove database lock")
                                    icon.name: "unlock"
                                },
                                Kirigami.Action {
                                    text: qsTr("Clear package cache")
                                    icon.name: "edit-clear-all"
                                },
                                Kirigami.Action {
                                    text: qsTr("Remove orphans")
                                    icon.name: "delete"
                                }
                            ]
                        }

                    }
                }

                GroupBox {
                    Layout.minimumWidth: 0
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0

                    background: Loader {
                        sourceComponent: groupBoxBackground
                    }

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            text: qsTr("Networking")
                            padding: Kirigami.Units.largeSpacing
                            font.weight: 600
                            opacity: 0.6
                        }

                        Kirigami.ActionToolBar {
                            Layout.fillWidth: true

                            actions: [
                                Kirigami.Action {
                                    text: qsTr("Rank mirrors")
                                    icon.name: "speedometer"
                                },
                                Kirigami.Action {
                                    text: qsTr("Change DNS server")
                                    icon.name: "network-server-database"
                                }
                            ]
                        }
                    }
                }


                GroupBox {
                    Layout.minimumWidth: 0
                    Layout.fillWidth: true
                    Layout.preferredWidth: 0

                    background: Loader {
                        sourceComponent: groupBoxBackground
                    }

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            text: qsTr("Additional")
                            padding: Kirigami.Units.largeSpacing
                            font.weight: 600
                            opacity: 0.6
                        }

                        Kirigami.ActionToolBar {
                            Layout.fillWidth: true

                            actions: [
                                Kirigami.Action {
                                    text: qsTr("Install Gaming packages")
                                    icon.name: "applications-games-symbolic"
                                },
                                Kirigami.Action {
                                    text: qsTr("Open KWin Debug window")
                                    icon.name: "debug-run"
                                }
                            ]
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
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
