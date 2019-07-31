import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"

TitlePane {
    id: root

    property variant repository: null

    title: Const.outputText

    content: Item {
        anchors.fill: parent

        StandardToolBar {
            id: buttons
            width: parent.width
            anchors.top: parent.top

            Row {
                spacing: Const.mainPadding

                StandardToolButton {
                    action: Action {
                        id: clearButton
                        text: Const.clearText
                        onTriggered: {
                            root.repository.clearOutput()
                        }
                    }
                }
            }
        }

        StandardStringListView {
            anchors.top: buttons.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            autoScrollToEnd: true
            model: root.repository.commandOutputModel
        }
    }
}
