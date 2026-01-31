import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami

HtmlReaderDialog {
    title: qsTr("Get involved")
    source: lsb != null ? ":/qt/qml/CachyOsHello/pages/" + lsb.currentLanguage.substring(0, 2) + "/involved.html" : ""
}
