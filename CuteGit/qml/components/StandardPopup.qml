import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import ".."

Popup {
    id: root
    height: content.computedHeight + Const.mainPadding * 2

    modal: true
    closePolicy: Popup.CloseOnEscape
    padding: Const.mainPadding

    Material.elevation: Const.popupElevation

    property alias title: title.text
    property alias controls: controlsLayout.children
    property alias buttons: buttonsLayout.children

    background: Rectangle {
        anchors.fill: parent
        color: Material.background
    }

    Component.onCompleted: {
        root.forceActiveFocus()
    }

    contentItem: Item {
        id: content

        property int computedHeight: title.height + controls.height + buttonToolBar.height

        Column {
            id: layout
            anchors.fill: parent

            StandardText {
                id: title
                width: parent.width
                height: Const.elementHeight * 2

                horizontalAlignment: Text.AlignHCenter
            }

            Item {
                id: controls
                width: parent.width
                height: controlsLayout.implicitHeight

                Column {
                    id: controlsLayout
                    anchors.fill: parent
                    spacing: Const.mainPadding
                }
            }

            StandardToolBar {
                id: buttonToolBar
                height: buttonsLayout.implicitHeight

                Row {
                    id: buttonsLayout
                    spacing: Const.mainPadding
                }
            }
        }
    }
}
