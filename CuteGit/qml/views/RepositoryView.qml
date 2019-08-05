import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import Qt.labs.platform 1.1 as QLP
import Qt.labs.folderlistmodel 2.1
import CuteGit 1.0
import "../js/Utils.js" as Utils
import "../components"
import "../pages"
import "../popups"

Pane {
    id: root
    padding: Const.panePadding

    property variant repository: null
    property bool filesAsTree: false

    Pane {
        id: repositoryStatus
        padding: 2
        anchors.margins: Const.paneMargins
        anchors.left: parent.left
        anchors.right: rightPart.left
        anchors.top: parent.top
        height: Const.elementHeight * 1.5

        Material.elevation: Const.paneElevation

        Rectangle {
            anchors.fill: parent

            color: root.repository
                   ? root.repository.repositoryStatus === CEnums.NoMerge
                     ? Const.transparent
                     : Material.accent
            : "black"
        }

        StandardText {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter

            color: root.repository ?
                       root.repository.repositoryStatus === CEnums.NoMerge
                       ? Material.foreground
                       : Material.background
            : "black"

            text: {
                if (root.repository)
                    if (root.repository.repositoryStatus === CEnums.InteractiveRebase)
                        qsTr("Interactive rebase in progress...")
                    else if (root.repository.repositoryStatus === CEnums.Rebase)
                        qsTr("Rebase in progress...")
                    else if (root.repository.repositoryStatus === CEnums.Merge)
                        qsTr("Merge in progress...")
                    else ""
                else ""
            }
        }
    }

    Item {
        id: centralPart
        anchors.top: repositoryStatus.bottom
        anchors.bottom: bottomPart.top
        anchors.left: parent.left
        anchors.right: rightPart.left

        ItemSelectionModel {
            id: treeFileSelection
            model: root.repository ? root.repository.treeFileModelProxy : null

            onCurrentIndexChanged: {
                root.repository.treeFileModelProxy.handleCurrentIndex(currentIndex)
            }
        }

        ItemSelectionModel {
            id: flatFileSelection
            model: root.repository ? root.repository.flatFileModelProxy : null

            onCurrentIndexChanged: {
                root.repository.flatFileModelProxy.handleCurrentIndex(currentIndex)
            }
        }

        FilePane {
            id: fileView
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * 0.5
            anchors.margins: Const.paneMargins
            filesAsTree: root.filesAsTree

            repository: root.repository
            treeSelection: treeFileSelection
            flatSelection: flatFileSelection
        }

        ToolPane {
            id: toolView
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: fileView.right
            anchors.right: parent.right
            anchors.margins: Const.paneMargins

            Material.elevation: Const.paneElevation

            repository: root.repository
        }
    }

    Item {
        id: rightPart
        anchors.top: parent.top
        anchors.bottom: bottomPart.top
        anchors.right: parent.right
        width: parent.width * 0.15

        BranchPane {
            id: branchView
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            repository: root.repository

            onRequestMergeBranch: {
                mergeBranchAction.branchName = name
                confirm.titleText = Const.mergeBranchText + " " + name
                confirm.messageText = Const.mergeBranchMessage.format(name)
                confirm.actionToTrigger = mergeBranchAction
                confirm.open()
            }

            onRequestDeleteBranch: {
                deleteBranchAction.branchName = name
                confirm.titleText = Const.deleteBranchText + " " + name
                confirm.messageText = Const.deleteBranchMessage
                confirm.actionToTrigger = deleteBranchAction
                confirm.open()
            }

            Action {
                id: mergeBranchAction
                property string branchName: ""
                onTriggered: root.repository.mergeBranch(branchName)
            }

            Action {
                id: deleteBranchAction
                property string branchName: ""
                onTriggered: root.repository.deleteBranch(branchName)
            }
        }
    }

    Item {
        id: bottomPart
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height * 0.33

        BranchLogPane {
            id: logView
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * 0.6

            repository: root.repository

            onRequestCopy: {
                root.repository.copy(commitId)
            }

            onRequestCommitDiffPrevious: {
                root.repository.commitDiffPrevious(commitId)
            }

            onRequestCommitBranchFrom: {
                branchFrom.commitId = commitId
                branchFrom.open()
            }

            onRequestCommitReset: {
                root.repository.commitReset(commitId)
            }

            onRequestCommitRebase: {
                root.repository.commitRebase(commitId)
            }

            onRequestCommitSquash: {
                root.repository.commitSquash(commitId)
            }

            onRequestCommitMessageChange: {
                commit.messageEnabled = true
                commit.messageText = commitMessage
                commit.showFileList = false
                commit.amend = false
                commit.commitId = commitId
                commit.open()
            }
        }

        OutputPane {
            id: outputView
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: logView.right
            anchors.right: parent.right
            anchors.margins: Const.paneMargins

            repository: root.repository
        }
    }

    ConfirmPopup {
        id: confirm
        width: root.width * Const.popupWidthSmall
        height: root.height * Const.popupHeightSmall
        anchors.centerIn: parent
    }

    CommitPopup {
        id: commit
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent

        repository: root.repository
    }

    BranchFromPopup {
        id: branchFrom
        width: root.width * Const.popupWidthSmall
        height: root.height * Const.popupHeightSmall
        anchors.centerIn: parent

        repository: root.repository
    }

    function requestRefresh() {
        root.repository.refresh()
    }

    function requestFetch() {
        root.repository.fetch()
    }

    function requestPull() {
        root.repository.pull()
    }

    function requestPush() {
        root.repository.push()
    }

    function requestStageAll() {
        root.repository.stageAll()
    }

    function requestUnstageAll() {
        root.repository.unstageAll()
    }

    function requestStageSelection() {
        root.repository.stageSelection(fileView.getSelectedFiles())
    }

    function requestUnstageSelection() {
        root.repository.unstageSelection(fileView.getSelectedFiles())
    }

    function requestRevertSelection() {
        root.repository.revertSelection(fileView.getSelectedFiles())
    }

    function requestStashSave() {
        root.repository.stashSave()
    }

    function requestStashPop() {
        root.repository.stashPop()
    }

    function requestCommit() {
        commit.messageEnabled = true
        commit.messageText = ""
        commit.showFileList = true
        commit.amend = false
        commit.open()
    }

    function requestAmend() {
        commit.messageEnabled = false
        commit.messageText = Const.amendingText
        commit.showFileList = true
        commit.amend = true
        commit.open()
    }

    function requestContinueRebase() {
        root.repository.continueRebase()
    }

    function requestAbortRebase() {
        root.repository.abortRebase()
    }
}
