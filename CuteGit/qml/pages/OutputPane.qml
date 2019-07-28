import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"

TitlePane {
    id: root

    property variant controller: null

    title: Const.outputText

    content: Item {
        anchors.fill: parent

        Item {
            id: toolbar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: Const.elementHeight + Const.smallPadding

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
            anchors.top: toolbar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            autoScrollToEnd: true
            model: root.controller.commandOutputModel
        }
    }
}
