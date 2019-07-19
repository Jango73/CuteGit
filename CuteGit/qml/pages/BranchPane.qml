import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import "../components"

Pane {
    id: root
    anchors.margins: Const.paneMargins

    Material.elevation: Const.paneElevation

    property variant controller: null

    StandardLabel {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Const.elementHeight
        text: Const.branchesText
    }

    StandardListView {
        id: list
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: count > 0
        model: root.controller.fileModel.branchModel

        delegate: Item {
            width: parent.width
            height: Const.elementHeight

            Item {
                anchors.fill: parent
                anchors.margins: Const.mainPadding * 0.5

                MouseArea {
                    anchors.fill: selection
                    onDoubleClicked: {
                        root.controller.fileModel.currentBranch = display
                    }
                }

                Selection {
                    id: selection
                    targetWidth: text.width
                    targetHeight: text.height
                    anchors.centerIn: text
                    visible: display === root.controller.fileModel.currentBranch
                }

                StandardText {
                    id: text
                    text: display
                    color: selection.visible ? Material.background : Material.foreground
                }
            }
        }
    }
}
