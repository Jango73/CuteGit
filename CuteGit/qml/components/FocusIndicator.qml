import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - Const.mainRadius * 2
        height: parent.height - Const.mainRadius * 2
        color: Material.foreground
        opacity: 0.1
    }
}
