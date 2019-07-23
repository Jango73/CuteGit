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

    LogView {
        id: logView
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: root.controller.treeFileModel !== null ? root.controller.treeFileModel.logModel : undefined

        onItemRightClicked: {
            menu.commitId = commitId
            menu.commitMessage = message
            menu.popup()
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
