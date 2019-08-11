import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - Const.mainPadding * 0.5
        height: parent.height - Const.mainPadding * 0.5
        color: Const.transparent
        border.width: 1
        border.color: Material.foreground
        opacity: 0.8
    }
}
