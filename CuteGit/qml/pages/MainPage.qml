import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import Qt.labs.platform 1.1 as QLP
import Qt.labs.folderlistmodel 2.1
import CuteGit 1.0
import "../components"

Item {
    id: root

    property variant controller: null

    MainMenu {
        id: menu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        controller: root.controller

        onRequestCloneRepository: clone.open()
        onRequestOpenRepository: folderDialog.open()

        onRequestStageSelection: {
            root.controller.repository.stageSelection(fileView.getSelectedFiles())
        }

        onRequestUnstageSelection: {
            root.controller.repository.unstageSelection(fileView.getSelectedFiles())
        }

        onRequestRevertSelection: {
            root.controller.repository.revertSelection(fileView.getSelectedFiles())
        }

        onRequestCommit: {
            commit.messageText = ""
            commit.showFileList = true
            commit.amend = false
            commit.open()
        }

        onRequestAmend: {
            commit.messageText = ""
            commit.showFileList = true
            commit.amend = true
            commit.open()
        }

        onRequestContinueRebase: root.controller.repository.continueRebase()
        onRequestAbortRebase:  root.controller.repository.abortRebase()

        onRequestShortcuts: shortcuts.open()
    }

    ToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: menu.bottom

        Row {
            ToolButton { action: menu.cloneRepositoryAction }
            ToolButton { action: menu.openRepositoryAction }
            ToolSeparator {}
            ToolButton { action: menu.pullAction }
            ToolButton { action: menu.pushAction }
            ToolSeparator {}
            ToolButton { action: menu.commitAction }
            ToolButton { action: menu.amendAction }
        }
    }

    Item {
        id: container
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Item {
            id: rightPart
            anchors.top: parent.top
            anchors.bottom: bottomPart.top
            anchors.right: parent.right
            width: parent.width * 0.15

            RepoPane {
                id: repositoryView
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: Const.paneMargins
                height: parent.height * 0.5

                controller: root.controller
            }

            BranchPane {
                id: branchView
                anchors.top: repositoryView.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                controller: root.controller
            }
        }

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
                color: root.controller.repository.repositoryStatus === CEnums.NoMerge
                       ? Const.transparent
                       : Material.accent
            }

            StandardText {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                color: root.controller.repository.repositoryStatus === CEnums.NoMerge
                       ? Material.foreground
                       : Material.background
                text: {
                    if (root.controller.repository.repositoryStatus === CEnums.InteractiveRebase)
                        qsTr("Interactive rebase in progress...")
                    else if (root.controller.repository.repositoryStatus === CEnums.Rebase)
                        qsTr("Rebase in progress...")
                    else if (root.controller.repository.repositoryStatus === CEnums.Merge)
                        qsTr("Merge in progress...")
                    else
                        ""
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
                id: fileSelection
                model: root.controller.repository.treeFileModelProxy

                onCurrentIndexChanged: {
                    root.controller.repository.treeFileModelProxy.handleCurrentIndex(currentIndex)
                }
            }

            FilePane {
                id: fileView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width * 0.5
                anchors.margins: Const.paneMargins
                filesAsTree: menu.filesAsTree

                controller: root.controller
                selection: fileSelection
            }

            ToolPane {
                id: toolView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: fileView.right
                anchors.right: parent.right
                anchors.margins: Const.paneMargins

                Material.elevation: Const.paneElevation

                controller: root.controller
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
                width: parent.width * 0.5

                controller: root.controller

                onRequestCommitDiffPrevious: {
                    root.controller.repository.commitDiffPrevious(commitId)
                }

                onRequestCommitBranchFrom: {
                    root.controller.repository.branchFromCommit(commitId)
                }

                onRequestCommitReset: {
                    root.controller.repository.commitReset(commitId)
                }

                onRequestCommitRebase: {
                    root.controller.repository.commitRebase(commitId)
                }

                onRequestCommitSquash: {
                    root.controller.repository.commitSquash(commitId)
                }

                onRequestCommitMessageChange: {
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

                controller: root.controller
            }
        }
    }

    ClonePopup {
        id: clone
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent

        controller: root.controller
    }

    CommitPopup {
        id: commit
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent

        controller: root.controller
    }

    Popup {
        id: shortcuts
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent
        modal: true
        closePolicy: Popup.CloseOnEscape
        padding: Const.mainPadding

        StandardText {
            anchors.fill: parent
            text: Const.shortcutsText
        }
    }

    QLP.FolderDialog {
        id: folderDialog

        onAccepted: {
            root.controller.repositoryPath = folder
        }
    }
}
