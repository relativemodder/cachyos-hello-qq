import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

Kirigami.Dialog {
    id: root
    standardButtons: Kirigami.Dialog.Ok
    padding: Kirigami.Units.largeSpacing
    preferredWidth: Kirigami.Units.gridUnit * 20

    property string source: ""

    function updateContent() {
        console.log("updateContent()");

        if (source == "") {
            textElement.text = "No content";
            return;
        }

        if (!fileLoader) {
            textElement.text = "";
            return;
        }

        var content = fileLoader.readFile(root.source);
        textElement.text = content.replace(/\n/g, "<br>");

        console.log("Loaded source " + root.source);
    }

    onSourceChanged: updateContent()
    Component.onCompleted: updateContent()

    Text {
        id: textElement
        width: parent.width
        color: Kirigami.Theme.textColor
        textFormat: Text.RichText

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
