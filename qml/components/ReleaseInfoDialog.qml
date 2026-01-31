import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

HtmlReaderDialog {
    title: qsTr("Release info")
    source: ":/qt/qml/CachyOsHello/pages/" + lsb.currentLanguage.substring(0, 2) + "/release.html"
}
