import QtQuick
import QtQuick.Controls
import org.kde.desktop as KDEDesktop

KDEDesktop.Switch {
    id: control

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.palette.windowText
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing

        wrapMode: Text.WordWrap
        width: control.width - control.indicator.width - control.spacing
    }
}
