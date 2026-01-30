import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.Page {
    id: root
    title: qsTr("Welcome to CachyOS!")

    actions: [
        Kirigami.Action {
            id: navigateToTweaksActions
            icon.name: "tools-wizard"
            text: qsTr("Tweaks and fixes")
            onTriggered: root.tweaks()
        }
    ]

    signal tweaks();
    signal next();

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        Kirigami.Heading {
            text: qsTr("Thank you for joining our community!")
        }

        Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: qsTr("We, the CachyOS Developers, hope that you will enjoy using CachyOS as much as we enjoy building it. Press \"Next\" to get you started with your new operating system. So enjoy the experience, and don't hesitate to send us your feedback.")
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Image {
                source: "qrc:/qt/qml/CachyOsHello/img/icon.png"
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            ComboBox {
                id: languageComboBox
                Layout.alignment: Qt.AlignLeft

                model: [
                    { text: "English", code: "en" },
                    { text: "Русский", code: "ru" }
                ]

                textRole: "text"
                valueRole: "code"

                Component.onCompleted: {
                    currentIndex = indexOfValue(lsb.currentLanguage.substring(0, 2));
                }

                Connections {
                    target: lsb
                    function onCurrentLanguageChanged() {
                        let lang = lsb.currentLanguage.substring(0, 2);
                        languageComboBox.currentIndex = languageComboBox.indexOfValue(lang);
                    }
                }

                onActivated: function(index) {
                    lsb.switchLanguage(currentValue);
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Button {
                leftPadding: 20
                rightPadding: 20
                icon.name: "gtk-quit"
                onClicked: Qt.quit()
                text: qsTr("Quit")
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
