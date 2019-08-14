import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root
    width: labelText.width + Const.mainPadding

    property alias text: labelText.text

    Selection {
        anchors.fill: parent
        anchors.topMargin: Const.smallPadding
        anchors.bottomMargin: Const.smallPadding
        color: Material.accent
        show: true
    }

    StandardText {
        id: labelText
        anchors.centerIn: parent
        color: Material.background
    }
}
