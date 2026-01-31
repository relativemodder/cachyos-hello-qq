import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

HtmlReaderDialog {
    title: qsTr("Read me")
    source: ":/qt/qml/CachyOsHello/pages/" + lsb.currentLanguage.substring(0, 2) + "/readme.html"
}
