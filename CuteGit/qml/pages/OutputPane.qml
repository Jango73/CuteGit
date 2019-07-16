import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"

Pane {
    id: root
    padding: 0

    Material.elevation: Const.paneElevation

    property variant controller: null

    StandardLabel {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width * 0.8
        height: toolbar.height
        text: Const.outputText
    }

    Item {
        id: toolbar
        anchors.top: parent.top
        anchors.left: title.right
        anchors.right: parent.right
        height: clearButton.height * 1.1

        RowLayout {
            id: buttons
            anchors.fill: parent

            StandardButton {
                id: clearButton
                Layout.alignment: Qt.AlignCenter
                text: Const.clearText

                onClicked: {
                    root.controller.clearOutput()
                }
            }
        }
    }

    StandardStringListView {
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        autoScrollToEnd: true
        model: root.controller.commandOutputModel
    }
}
