import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    property Item listView: null
    property alias text: theText.text
    property alias selectionVisible: selection.visible

    signal clicked(var mouse)
    signal doubleClicked(var mouse)

    Item {
        anchors.fill: parent
        anchors.margins: Const.smallPadding

        MouseArea {
            anchors.fill: selection
            acceptedButtons: Qt.AllButtons
            onClicked: {
                root.listView.currentIndex = index
                root.listView.forceActiveFocus()
                root.clicked(mouse)
            }
            onDoubleClicked: {
                root.listView.currentIndex = index
                root.listView.forceActiveFocus()
                root.doubleClicked(mouse)
            }
        }

        Selection {
            id: selection
            targetWidth: theText.width
            targetHeight: theText.height
            anchors.centerIn: theText
        }

        FocusIndicator {
            anchors.fill: selection
            visible: root.listView.activeFocus && index === root.listView.currentIndex
        }

        TextOverSelection {
            id: theText
            width: parent.width - Const.smallPadding
            selection: selection
        }
    }
}
