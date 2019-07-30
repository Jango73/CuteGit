import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    property alias text: theText.text
    property alias selectionVisible: selection.visible

    signal clicked(var mouse)
    signal doubleClicked(var mouse)

    Item {
        anchors.fill: parent
        anchors.margins: Const.smallPadding

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            onClicked: root.clicked(mouse)
            onDoubleClicked: root.doubleClicked(mouse)
        }

        Selection {
            id: selection
            targetWidth: theText.width
            targetHeight: theText.height
            anchors.centerIn: theText
        }

        ElideText {
            id: theText
            width: parent.width - Const.smallPadding
            color: selection.visible ? Material.background : Material.foreground
        }
    }
}
