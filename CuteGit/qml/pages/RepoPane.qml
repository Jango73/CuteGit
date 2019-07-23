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
        text: Const.repositoriesText
    }

    StandardLabel {
        id: listPlaceHolder
        anchors.fill: list
        visible: !list.visible
        text: "Open a repository..."
    }

    StandardListView {
        id: list
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: count > 0

        model: root.controller.repositoryModel

        delegate: Item {
            width: parent.width
            height: Const.elementHeight

            Item {
                anchors.fill: parent
                anchors.margins: Const.smallPadding

                MouseArea {
                    anchors.fill: selection
                    onDoubleClicked: {
                        root.controller.repositoryPath = display
                    }
                }

                Selection {
                    id: selection
                    targetWidth: text.width
                    targetHeight: text.height
                    anchors.centerIn: text
                    visible: display === root.controller.repositoryPath
                }

                ElideText {
                    id: text
                    width: parent.width - Const.smallPadding
                    text: display.split("/").slice(-1)[0]
                    color: selection.visible ? Material.background : Material.foreground
                }
            }
        }
    }
}
