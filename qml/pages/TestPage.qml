import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.Page {
    id: root
    title: qsTr("Test page")

    signal back();
    signal next();

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

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
