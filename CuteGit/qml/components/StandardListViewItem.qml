import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    property Item listView: null
    property alias text: theText.text
    property alias selectionShown: selection.show

    signal clicked(var mouse)
    signal doubleClicked(var mouse)

    Item {
        anchors.fill: parent
        anchors.margins: Const.verySmallPadding

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
            anchors.fill: parent
        }

        FocusIndicator {
            anchors.fill: parent
            visible: root.listView.activeFocus && index === root.listView.currentIndex
        }

        TextOverSelection {
            id: theText
            anchors.centerIn: parent
            width: parent.width - Const.mainPadding
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            selection: selection
        }
    }
}
