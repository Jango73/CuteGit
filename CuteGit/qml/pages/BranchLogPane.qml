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

    signal requestCommitRebase(var commitId)
    signal requestCommitMessageChange(var commitId, var commitMessage)

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
        activeFocusOnTab: true

        model: root.controller.treeFileModel !== null ? root.controller.treeFileModel.logModel : undefined

        delegate: Item {
            id: delegateItem
            width: parent.width
            height: Const.elementHeight + Const.smallPadding

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.AllButtons
                onClicked: {
                    list.currentIndex = index

                    if (mouse.button === Qt.RightButton) {
                        menu.commitId = model.commitId
                        menu.commitMessage = model.message
                        menu.popup()
                    }
                }
            }

            Selection {
                id: selection
                targetWidth: data.width
                targetHeight: data.height
                anchors.centerIn: data
                visible: index === list.currentIndex

                FocusIndicator {
                    anchors.fill: parent
                    visible: list.activeFocus
                }
            }

            Item {
                id: data
                anchors.centerIn: parent
                width: parent.width - Const.smallPadding
                height: parent.height - Const.smallPadding

                ElideText {
                    id: messageField
                    width: parent.width * 0.55
                    verticalAlignment: Text.AlignVCenter
                    color: selection.visible ? Material.background : Material.foreground
                    text: model.message
                }

                ElideText {
                    id: authorField
                    anchors.left: messageField.right
                    width: parent.width * 0.25
                    verticalAlignment: Text.AlignVCenter
                    color: selection.visible ? Material.background : Material.foreground
                    text: model.author
                }

                ElideText {
                    id: dateField
                    anchors.left: authorField.right
                    width: parent.width * 0.2
                    verticalAlignment: Text.AlignVCenter
                    color: selection.visible ? Material.background : Material.foreground
                    text: model.date
                }
            }
        }
    }

    LogMenu {
        id: menu
        controller: root.controller

        onRequestCommitRebase: {
            root.requestCommitRebase(commitId)
        }

        onRequestCommitMessageChange: {
            root.requestCommitMessageChange(commitId, commitMessage)
        }
    }
}
