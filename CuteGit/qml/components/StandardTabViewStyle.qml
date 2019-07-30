import QtQuick 2.12
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

TabViewStyle {
    frameOverlap: 1

    tab: Rectangle {
        color: styleData.selected ? Material.primary : Material.background
        implicitWidth: Math.max(text.width + 4, 80)
        implicitHeight: Const.elementHeight
        radius: Const.mainRadius

        FocusIndicator {
            anchors.fill: parent
            visible: styleData.activeFocus
        }

        StandardText {
            id: text
            anchors.centerIn: parent
            text: styleData.title
            color: styleData.selected ? Material.background : Material.foreground
        }
    }

    frame: Rectangle {
        color: Material.background
    }
}
