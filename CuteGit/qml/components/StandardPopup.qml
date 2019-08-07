import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "."

Popup {
    id: root
    modal: true
    closePolicy: Popup.CloseOnEscape
    padding: Const.mainPadding

    Material.elevation: Const.popupElevation

    background: Rectangle {
        anchors.fill: parent
        color: Material.background
    }

    Component.onCompleted: {
        root.forceActiveFocus()
    }
}
