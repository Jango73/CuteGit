import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

MenuBar {
    id: root

    Material.elevation: 4

    property variant controller: null
    property bool filesAsTree: false

    property bool rebaseInProgress: controller.repository.repositoryStatus === CEnums.Rebase
    || controller.repository.repositoryStatus === CEnums.InteractiveRebase

    property alias cloneRepositoryAction: cloneRepository
    property alias openRepositoryAction: openRepository
    property alias pullAction: pull
    property alias pushAction: push
    property alias commitAction: commit
    property alias amendAction: amend

    signal requestCloneRepository()
    signal requestOpenRepository()
    signal requestStageSelection()
    signal requestUnstageSelection()
    signal requestRevertSelection()
    signal requestCommit()
    signal requestAmend()
    signal requestContinueRebase()
    signal requestAbortRebase()
    signal requestShortcuts()

    Menu {
        title: qsTr("&Repository")

        Action {
            id: cloneRepository
            text: qsTr("&Clone")
            enabled: root.controller.repository.can(CEnums.Clone)
            onTriggered: root.requestCloneRepository()
        }

        Action {
            id: openRepository
            text: qsTr("&Open")
            shortcut: "Ctrl+O"
            onTriggered: root.requestOpenRepository()
        }

        Action {
            text: qsTr("&Remove")
        }

        MenuSeparator { }

        Action {
            text: qsTr("&Quit")
            shortcut: "Ctrl+Q"
            onTriggered: root.controller.quit()
        }
    }

    Menu {
        title: qsTr("Re&mote")

        Action {
            id: fetch
            text: qsTr("&Fetch")
            shortcut: "Ctrl+F"
            enabled: root.controller.repository.can(CEnums.Fetch)
            onTriggered: root.controller.repository.fetch()
        }

        Action {
            id: pull
            text: qsTr("&Pull")
            shortcut: "Ctrl+L"
            enabled: root.controller.repository.can(CEnums.Pull)
            onTriggered: root.controller.repository.pull()
        }

        Action {
            id: push
            text: qsTr("Pus&h")
            shortcut: "Ctrl+P"
            enabled: root.controller.repository.can(CEnums.Push)
            onTriggered: root.controller.repository.push()
        }
    }

    Menu {
        title: qsTr("&Local")

        Action {
            text: qsTr("Stage a&ll")
            shortcut: "Ctrl+shift++"
            onTriggered: root.controller.repository.stageAll()
        }

        Action {
            text: qsTr("Usta&ge all")
            shortcut: "Ctrl+shift+-"
            onTriggered: root.controller.repository.unstageAll()
        }

        Action {
            text: qsTr("&Stage selection")
            shortcut: "Ctrl++"
            onTriggered: root.requestStageSelection()
        }

        Action {
            text: qsTr("&Unstage selection")
            shortcut: "Ctrl+-"
            onTriggered: root.requestUnstageSelection()
        }

        Action {
            text: qsTr("&Revert selection")
            shortcut: "Ctrl+Z"
            onTriggered: root.requestRevertSelection()
        }

        Action {
            id: commit
            text: qsTr("&Commit")
            shortcut: "Ctrl+C"
            enabled: root.controller.repository.can(CEnums.Commit)
            onTriggered: root.requestCommit()
        }

        Action {
            id: amend
            text: qsTr("&Amend")
            shortcut: "Ctrl+A"
            enabled: root.controller.repository.can(CEnums.Amend)
            onTriggered: root.requestAmend()
        }

        Action {
            text: qsTr("C&ontinue rebase")
            shortcut: "Ctrl+R"
            enabled: root.rebaseInProgress && root.controller.repository.can(CEnums.ContinueRebase)
            onTriggered: root.requestContinueRebase()
        }

        Action {
            text: qsTr("Abor&t rebase")
            shortcut: "Ctrl+T"
            enabled: root.rebaseInProgress && root.controller.repository.can(CEnums.AbortRebase)
            onTriggered: root.requestAbortRebase()
        }
    }

    Menu {
        title: qsTr("&View")

        Action {
            text: qsTr("&Refresh")
            shortcut: "F5"
            onTriggered: root.controller.repository.refresh()
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
