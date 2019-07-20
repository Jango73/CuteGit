import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
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
        text: Const.logText
    }

    StandardListView {
        id: list
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        model: root.controller.fileModel !== null ? root.controller.fileModel.logModel : undefined

        delegate: Item {
            id: delegateItem
            width: parent.width
            height: Const.elementHeight

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    list.currentIndex = index
                }
            }

            Selection {
                id: selection
                targetWidth: parent.width
                targetHeight: parent.height
                anchors.centerIn: parent
                visible: index === list.currentIndex
            }

            StandardText {
                id: messageField
                width: parent.width * 0.55
                text: model.message
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                color: selection.visible ? Material.background : Material.foreground
            }

            StandardText {
                id: authorField
                width: parent.width * 0.25
                anchors.left: messageField.right
                text: model.author
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                color: selection.visible ? Material.background : Material.foreground
            }

            StandardText {
                id: dateField
                width: parent.width * 0.2
                anchors.left: authorField.right
                text: model.date
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                color: selection.visible ? Material.background : Material.foreground
            }

//                StandardText {
//                    text: commitId
//                    elide: Text.ElideRight
//                }
        }
    }
}
