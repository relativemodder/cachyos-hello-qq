import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import "../components"
import org.cachyos.tweaks

Kirigami.Page {
    id: root
    title: qsTr("Tweaks and fixes")

    signal close()

    readonly property var tweaksModel: [
        { id: "psd", text: qsTr("Use RAM for browser profiles") },
        { id: "ananicy", text: qsTr("Auto-adjust process priorities for better system responsiveness") },
        { id: "cachy-update", text: qsTr("Enable CachyOS update notifier in tray") },
        { id: "oomd", text: qsTr("Proactively kill processes during low memory to prevent freezes") },
        { id: "bpftune", text: qsTr("Automatically tune system network using Bpftune") },
        { id: "bluetooth", text: qsTr("Enable Bluetooth support") }
    ]

    TweakController {
        id: tweakController

        onOperationFinished: (tweakId, success, message) => {
            if (!success) {
                console.error("Tweak operation failed:", message);
                showMessage(message, false);
            }
        }

        onTweakStateChanged: (tweakId, enabled) => {
            console.log("Tweak", tweakId, enabled ? "enabled" : "disabled");
            updateRepeater(tweakId);
        }

        onTweakStateNeedsRefresh: (tweakId) => {
            console.log("Tweak", tweakId, "needs to refresh");
            updateRepeater(tweakId);
        }
    }

    function updateRepeater(tweakId) {
        for (let i = 0; i < tweaksRepeater.count; i++) {
            let item = tweaksRepeater.itemAt(i);
            if (item && item.tweakId === tweakId) {
                item.checked = tweakController.isTweakEnabled(tweakId);
                break;
            }
        }
    }

    Connections {
        target: tweakController.pacmanService

        function onOperationStarted(operation) {
            console.log("operation started");
        }

        function onOperationFinished(success, message) {
            if (message !== "") {
                showMessage(message, success);
            }
        }
    }

    Connections {
        target: tweakController.networkingFixService

        function onOperationStarted(operation) {
            console.log("Networking operation started:", operation);
        }

        function onOperationFinished(success, message) {
            showMessage(message, success);
        }
    }

    function showMessage(text, isSuccess) {
        inlineMessage.text = text;
        inlineMessage.type = isSuccess ? Kirigami.MessageType.Positive : Kirigami.MessageType.Error;
        inlineMessage.visible = true;
    }

    Menu {
        id: dnsMenu

        MenuItem {
            text: "Cloudflare (1.1.1.1)"
            onTriggered: tweakController.networkingFixService.changeDNS("cloudflare")
        }
        MenuItem {
            text: "Google (8.8.8.8)"
            onTriggered: tweakController.networkingFixService.changeDNS("google")
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Kirigami.InlineMessage {
            id: inlineMessage
            Layout.fillWidth: true
            showCloseButton: true
        }

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

                Repeater {
                    id: tweaksRepeater
                    model: root.tweaksModel
                    delegate: WrapSwitch {
                        id: tweakSwitch
                        text: modelData.text
                        property string tweakId: modelData.id
                        checked: tweakController.isTweakEnabled(tweakId)
                        onToggled: tweakController.toggleTweak(tweakId)
                        enabled: !tweakController.commandRunner.isRunning
                    }
                }

                Kirigami.Heading {
                    text: qsTr("Fixes")
                    Layout.topMargin: Kirigami.Units.largeSpacing * 1.5
                }

                FixGroupBox {
                    labelText: qsTr("Package management")

                    Kirigami.ActionToolBar {
                        Layout.fillWidth: true
                        actions: [
                            Kirigami.Action {
                                text: qsTr("System update")
                                icon.name: "system-reboot-update"
                                onTriggered: tweakController.pacmanService.updateSystem()
                            },
                            Kirigami.Action {
                                text: qsTr("Reinstall all packages")
                                icon.name: "system-reboot"
                                onTriggered: tweakController.pacmanService.reinstallAllPackages()
                            },
                            Kirigami.Action {
                                text: qsTr("Reset keyrings")
                                icon.name: "edit-undo"
                                onTriggered: tweakController.pacmanService.resetKeyrings()
                            }
                        ]
                    }

                    Kirigami.ActionToolBar {
                        Layout.fillWidth: true
                        actions: [
                            Kirigami.Action {
                                text: qsTr("Remove database lock")
                                icon.name: "unlock"
                                onTriggered: tweakController.pacmanService.removeDatabaseLock()
                            },
                            Kirigami.Action {
                                text: qsTr("Clear package cache")
                                icon.name: "edit-clear-all"
                                onTriggered: tweakController.pacmanService.clearCache()
                            },
                            Kirigami.Action {
                                text: qsTr("Remove orphans")
                                icon.name: "delete"
                                onTriggered: tweakController.pacmanService.removeOrphans()
                            }
                        ]
                    }
                }

                FixGroupBox {
                    labelText: qsTr("Networking")

                    Kirigami.ActionToolBar {
                        Layout.fillWidth: true
                        actions: [
                            Kirigami.Action {
                                text: qsTr("Rank mirrors")
                                icon.name: "speedometer"
                                onTriggered: tweakController.networkingFixService.rankMirrors()
                            },
                            Kirigami.Action {
                                text: qsTr("Change DNS server")
                                icon.name: "network-server-database"
                                onTriggered: dnsMenu.popup()
                            }
                        ]
                    }
                }

                FixGroupBox {
                    labelText: qsTr("Additional")

                    Kirigami.ActionToolBar {
                        Layout.fillWidth: true
                        actions: [
                            Kirigami.Action {
                                text: qsTr("Install Gaming packages")
                                icon.name: "applications-games-symbolic"
                                onTriggered: tweakController.pacmanService.installPackages(["cachyos-gaming-meta", "cachyos-gaming-applications"])
                            },
                            Kirigami.Action {
                                text: qsTr("Open KWin Debug window")
                                icon.name: "debug-run"
                                onTriggered: tweakController.kwinService.showDebugConsole()
                            }
                        ]
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
