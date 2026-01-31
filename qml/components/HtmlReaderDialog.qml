import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Kirigami.Dialog {
    id: root
    standardButtons: Kirigami.Dialog.Ok
    padding: Kirigami.Units.largeSpacing
    preferredWidth: Kirigami.Units.gridUnit * 20

    property string source: "";

    Text {
        width: parent.width
        color: Kirigami.Theme.textColor
        textFormat: Text.RichText
        text: {
            if (source == "") {
                return "No content";
            }

            if (!fileLoader) {
                return "";
            }

            var content = fileLoader.readFile(root.source);
            return content.replace(/\n/g, "<br>");
        }
        onLinkActivated: function(link) {
            Qt.openUrlExternally(link)
        }

        wrapMode: Text.WordWrap

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }
}
