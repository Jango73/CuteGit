import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQml 2.12
import CuteGit 1.0
import "../components"

MenuBar {
    id: root

    Material.elevation: 4

    property Item repositoryView: null
    property variant controller: null
    property variant repository: null
    property bool filesAsTree: false

    property bool rebaseInProgress: root.repository.repositoryStatus === CEnums.Rebase
                                    || root.repository.repositoryStatus === CEnums.InteractiveRebase

    property alias cloneRepositoryAction: cloneRepository
    property alias openRepositoryAction: openRepository
    property alias pullAction: pull
    property alias pushAction: push
    property alias commitAction: commit
    property alias amendAction: amend

    signal requestCloneRepository()
    signal requestOpenRepository()
    signal requestShortcuts()

    Menu {
        id: repositoryMenu
        title: Const.repositoryMenuText

        Action {
            id: cloneRepository
            text: Const.cloneMenuText
            enabled: root.repository.can(CEnums.Clone)
            onTriggered: root.requestCloneRepository()
        }

        Action {
            id: openRepository
            text: Const.openMenuText
            shortcut: "Ctrl+O"
            onTriggered: root.requestOpenRepository()
        }

        Action {
            text: Const.removeMenuText
        }

        Menu {
            id: knownRepos
            title: Const.knownMenuText

            Instantiator {
                model: root.controller.knownRepositoryModel

                MenuItem {
                    text: root.controller.repositoryNameFromPath(model.display)

                    onClicked: {
                        // If calling root.controller.openRepository(model.display) here, menu does not close...
                        // It must be missing some signal
                        openRepositoryTimer.start()
                    }

                    Timer {
                        id: openRepositoryTimer
                        repeat: false
                        interval: 500
                        onTriggered: root.controller.openRepository(model.display)
                    }
                }

                onObjectAdded: knownRepos.insertItem(index, object)
                onObjectRemoved: knownRepos.removeItem(object)
            }
        }

        MenuSeparator { }

        Action {
            text: Const.quitMenuText
            shortcut: "Ctrl+Q"
            onTriggered: root.controller.quit()
        }
    }

    Menu {
        title: Const.remoteMenuText

        Action {
            id: fetch
            text: Const.fetchMenuText
            shortcut: "Ctrl+F"
            enabled: root.repository.can(CEnums.Fetch)
            onTriggered: repositoryView.requestFetch()
        }

        Action {
            id: pull
            text: Const.pullMenuText
            shortcut: "Ctrl+L"
            enabled: root.repository.can(CEnums.Pull)
            onTriggered: repositoryView.requestPull()
        }

        Action {
            id: push
            text: Const.pushMenuText
            shortcut: "Ctrl+P"
            enabled: root.repository.can(CEnums.Push)
            onTriggered: repositoryView.requestPush()
        }
    }

    Menu {
        title: qsTr("&Local")

        Action {
            text: qsTr("Stage a&ll")
            shortcut: "Ctrl+shift++"
            onTriggered: repositoryView.requestStageAll()
        }

        Action {
            text: qsTr("Usta&ge all")
            shortcut: "Ctrl+shift+-"
            onTriggered: repositoryView.requestUnstageAll()
        }

        Action {
            text: qsTr("&Stage selection")
            shortcut: "Ctrl++"
            onTriggered: repositoryView.requestStageSelection()
        }

        Action {
            text: qsTr("&Unstage selection")
            shortcut: "Ctrl+-"
            onTriggered: repositoryView.requestUnstageSelection()
        }

        Action {
            text: qsTr("&Revert selection")
            shortcut: "Ctrl+Z"
            onTriggered: repositoryView.requestRevertSelection()
        }

        Action {
            text: qsTr("Save stash")
            enabled: root.repository.can(CEnums.Stash)
            onTriggered: repositoryView.requestStashSave()
        }

        Action {
            text: qsTr("Pop stash")
            enabled: root.repository.can(CEnums.Stash)
            onTriggered: repositoryView.requestStashPop()
        }

        Action {
            id: commit
            text: qsTr("&Commit")
            shortcut: "Ctrl+C"
            enabled: root.repository.can(CEnums.Commit) && root.repository.hasCommitableFiles
            onTriggered: repositoryView.requestCommit()
        }

        Action {
            id: amend
            text: qsTr("&Amend")
            shortcut: "Ctrl+A"
            enabled: root.repository.can(CEnums.Amend) && root.repository.hasCommitableFiles
            onTriggered: repositoryView.requestAmend()
        }

        Action {
            text: qsTr("C&ontinue rebase")
            shortcut: "Ctrl+R"
            enabled: root.rebaseInProgress && root.repository.can(CEnums.ContinueRebase)
            onTriggered: repositoryView.requestContinueRebase()
        }

        Action {
            text: qsTr("Abor&t rebase")
            shortcut: "Ctrl+T"
            enabled: root.rebaseInProgress && root.repository.can(CEnums.AbortRebase)
            onTriggered: repositoryView.requestAbortRebase()
        }
    }

    Menu {
        title: qsTr("&View")

        Action {
            text: qsTr("&Refresh")
            shortcut: "F5"
            onTriggered: repositoryView.requestRefresh()
        }

        MenuItem {
            text: qsTr("Files as a tree")
            checkable: true
            checked: root.filesAsTree
            onClicked: root.filesAsTree = !root.filesAsTree
        }

        MenuItem {
            text: qsTr("Show &clean")
            checkable: true
            checked: root.controller.showClean
            onClicked: root.controller.showClean = !root.controller.showClean
        }

        MenuItem {
            text: qsTr("Show &added")
            checkable: true
            checked: root.controller.showAdded
            onClicked: root.controller.showAdded = !root.controller.showAdded
        }

        MenuItem {
            text: qsTr("Show &modified")
            checkable: true
            checked: root.controller.showModified
            onClicked: root.controller.showModified = !root.controller.showModified
        }

        MenuItem {
            text: qsTr("Show &deleted")
            checkable: true
            checked: root.controller.showDeleted
            onClicked: root.controller.showDeleted = !root.controller.showDeleted
        }

        MenuItem {
            text: qsTr("Show &untracked")
            checkable: true
            checked: root.controller.showUntracked
            onClicked: root.controller.showUntracked = !root.controller.showUntracked
        }
    }

    Menu {
        title: qsTr("&Help")

        Action {
            text: qsTr("&Shortcuts")
            onTriggered: root.requestShortcuts()
        }
    }
}
