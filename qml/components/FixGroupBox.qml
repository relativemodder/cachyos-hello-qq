import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

GroupBox {
    id: root

    property string labelText: ""

    Layout.minimumWidth: 0
    Layout.fillWidth: true
    Layout.preferredWidth: 0

    background: Rectangle {
        color: "transparent"
        border.width: 1
        Kirigami.Theme.colorSet: Kirigami.Theme.Header
        border.color: Kirigami.Theme.disabledTextColor
        radius: Kirigami.Units.cornerRadius
        opacity: 0.5
    }

    contentItem: ColumnLayout {
        spacing: 0

        Label {
            text: root.labelText
            padding: Kirigami.Units.largeSpacing
            font.weight: 600
            opacity: 0.6
        }

        ColumnLayout {
            id: contentColumn
            Layout.fillWidth: true
            spacing: 0
        }
    }

    default property alias _content: contentColumn.data
}
