import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import Qt.labs.platform 1.1 as QLP
import Qt.labs.folderlistmodel 2.1
import CuteGit 1.0
import "../generalUtils.js" as Utils
import "../components"
import "../pages"
import "../popups"

Pane {
    id: root
    padding: Const.panePadding

    property variant repository: null

    //--------------------------------------------------------------------------------
    // Status

    StandardPane {
        id: repositoryStatus
        anchors.left: parent.left
        anchors.right: rightPart.left
        anchors.top: parent.top
        height: Const.elementHeight * 1.5

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

            color: root.repository
                   ? root.repository.repositoryStatus === CEnums.NoMerge
                     ? Material.foreground
                     : Material.background
            : "black"

            text: root.repository
                  ?
                      root.repository.repositoryStatus === CEnums.InteractiveRebase
                      ? Const.interactiveRebaseProgressText
                      : root.repository.repositoryStatus === CEnums.Rebase
                        ? Const.rebaseProgressText
                        : root.repository.repositoryStatus === CEnums.Merge
                          ? Const.mergeProgressText
                          : ""
                      + " - " + root.repository.repositoryTypeString
                      + qsTr(" - ( Ahead ") + root.repository.commitCountAhead + qsTr(" : behind ") + root.repository.commitCountBehind + " )"
            : ""
        }
    }

    //--------------------------------------------------------------------------------
    // File view and tools

    Item {
        id: centralPart
        anchors.top: repositoryStatus.bottom
        anchors.bottom: bottomPart.top
        anchors.left: parent.left
        anchors.right: rightPart.left

        ItemSelectionModel {
            id: flatFileSelection
            model: root.repository ? root.repository.flatFileModelProxy : null

            onCurrentIndexChanged: {
                root.repository.flatFileModelProxy.handleCurrentIndex(currentIndex)
            }
        }

        FilePane {
            id: filePane
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * 0.5
            anchors.margins: Const.paneMargins

            extensionParent: root
            repository: root.repository
            flatSelection: flatFileSelection

            onRequestMenu: {
                fileMenu.name = name
                fileMenu.popup()
            }

            onRequestDeleteFile: {
                root.requestDeleteFile(name)
            }

            onRequestFileFilter: {
                root.repository.setFileFilter(text)
            }
        }

        ToolPane {
            id: toolPane
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: filePane.right
            anchors.right: parent.right
            anchors.margins: Const.paneMargins

            extensionParent: root
            repository: root.repository

            onRequestMenu: {
                logMenu.commitId = commitId
                logMenu.commitMessage = message
                logMenu.popup()
            }
        }
    }

    Item {
        id: rightPart
        anchors.top: parent.top
        anchors.bottom: bottomPart.top
        anchors.right: parent.right
        width: parent.width * 0.2

        BranchPane {
            id: branchPane
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            repository: root.repository

            onRequestMergeBranch: {
                mergeBranchAction.branchName = name
                confirm.title = Const.mergeBranchText + " " + name
                confirm.messageText = Const.mergeBranchMessage.arg(name)
                confirm.actionOnAccept = mergeBranchAction
                confirm.actionOnReject = null
                confirm.open()
            }

            onRequestDeleteBranch: {
                deleteBranchAction.branchName = name
                confirm.title = Const.deleteBranchText + " " + name
                confirm.messageText = Const.deleteBranchMessage
                confirm.actionOnAccept = deleteBranchAction
                confirm.actionOnReject = null
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
        height: parent.height * 0.4

        BranchLogPane {
            id: branchLogPane
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * 0.6

            extensionParent: root
            repository: root.repository

            onRequestMenu: {
                logMenu.commitId = commitId
                logMenu.commitMessage = message
                logMenu.popup()
            }

            onRequestCommitDiffPrevious: {
                root.commitDiffPrevious(commitId)
            }
        }

        OutputPane {
            id: outputPane
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: branchLogPane.right
            anchors.right: parent.right
            anchors.margins: Const.paneMargins

            extensionParent: root
            repository: root.repository
        }
    }

    //--------------------------------------------------------------------------------
    // Menus

    LogMenu {
        id: logMenu
        repository: root.repository

        onRequestCopy: {
            root.repository.copy(commitId)
        }

        onRequestCommitDiffPrevious: {
            root.commitDiffPrevious(commitId)
        }

        onRequestCommitTagOn: {
            tagOn.commitId = commitId
            tagOn.open()
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

        onRequestCommitDiffFrom: {
            root.repository.diffFromCommitId = commitId
            root.activateFileDiffView()
        }

        onRequestCommitDiffTo: {
            root.repository.diffToCommitId = commitId
            root.activateFileDiffView()
        }
    }

    FileMenu {
        id: fileMenu
        repository: root.repository

        onRequestDelete: {
            root.requestDeleteFile(name)
        }
    }

    //--------------------------------------------------------------------------------
    // Popups

    ConfirmPopup {
        id: confirm
        anchors.centerIn: parent
    }

    CommitPopup {
        id: commit
        anchors.centerIn: parent

        repository: root.repository
    }

    TagOnPopup {
        id: tagOn
        anchors.centerIn: parent

        repository: root.repository
    }

    BranchFromPopup {
        id: branchFrom
        anchors.centerIn: parent

        repository: root.repository
    }

    Action {
        id: deleteFileAction
        property string fileName: ""
        onTriggered: root.repository.deleteFile(fileName)
    }

    //--------------------------------------------------------------------------------
    // Functions

    function requestDeleteFile(name) {
        deleteFileAction.fileName = name
        confirm.title = Const.deleteFileText
        confirm.messageText = Const.deleteFileMessage.arg(name)
        confirm.actionOnAccept = deleteFileAction
        confirm.actionOnReject = null
        confirm.open()
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
        root.repository.stageSelection(filePane.getSelectedFiles())
    }

    function requestUnstageSelection() {
        root.repository.unstageSelection(filePane.getSelectedFiles())
    }

    function requestRevertSelection() {
        root.repository.revertSelection(filePane.getSelectedFiles())
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

    function commitDiffPrevious(commitId) {
        root.repository.commitDiffPrevious(commitId)
        root.activateFileDiffView()
    }

    function activateFlatFileView() {
        filePane.activateFlatFileView()
    }

    function activateBranchLogView() {
        branchLogPane.activateBranchLogView()
    }

    function activateGraphView() {
        branchLogPane.activateGraphView()
    }

    function activateFileDiffView() {
        toolPane.activateFileDiffView()
    }
}
