import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - Const.mainPadding * 0.25
        height: parent.height - Const.mainPadding * 0.25
        color: Const.transparent
        border.width: 2
        border.color: "white"
        opacity: 0.8
        radius: Const.mainRadius
    }
}
