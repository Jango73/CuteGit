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
    property variant materialTheme: 0

    property bool rebaseInProgress: repository
                                    ? repository.repositoryStatus === CEnums.Rebase
                                      || repository.repositoryStatus === CEnums.InteractiveRebase
                                    : false

    property alias cloneRepositoryAction: cloneRepository
    property alias openRepositoryAction: openRepository
    property alias fetchAction: fetch
    property alias pullAction: pull
    property alias pushAction: push
    property alias stageSelectionAction: stageSelection
    property alias revertSelectionAction: revertSelection
    property alias commitAction: commit
    property alias amendAction: amend
    property alias saveStashAction: saveStash
    property alias popStashAction: popStash

    signal requestCloneRepository()
    signal requestOpenRepository()
    signal requestDarkTheme()
    signal requestLightTheme()
    signal requestHelp()

    Menu {
        id: repositoryMenu
        title: Const.repositoryMenuText

        Action {
            id: cloneRepository
            text: Const.cloneMenuText
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
                        // When calling root.controller.openRepository(model.display) from here, the menu does not close...
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
            enabled: root.repository ? root.repository.can(CEnums.Fetch) : false
            onTriggered: repositoryView.requestFetch()
        }

        Action {
            id: pull
            text: Const.pullMenuText
            shortcut: "Ctrl+L"
            enabled: root.repository ? root.repository.can(CEnums.Pull) && root.repository.hasPullableCommits : false
            onTriggered: repositoryView.requestPull()
        }

        Action {
            id: push
            text: Const.pushMenuText
            shortcut: "Ctrl+P"
            enabled: root.repository ? root.repository.can(CEnums.Push) && root.repository.hasPushableCommits : false
            onTriggered: repositoryView.requestPush()
        }
    }

    Menu {
        title: qsTr("&Local")

        Action {
            text: qsTr("Stage a&ll")
            shortcut: "Ctrl+shift++"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestStageAll()
        }

        Action {
            text: qsTr("Usta&ge all")
            shortcut: "Ctrl+shift+-"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestUnstageAll()
        }

        Action {
            id: stageSelection
            text: qsTr("&Stage selection")
            shortcut: "Ctrl++"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestStageSelection()
        }

        Action {
            text: qsTr("&Unstage selection")
            shortcut: "Ctrl+-"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestUnstageSelection()
        }

        MenuSeparator { }

        Action {
            id: revertSelection
            text: qsTr("&Revert selection")
            shortcut: "Ctrl+Z"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestRevertSelection()
        }

        MenuSeparator { }

        Action {
            id: saveStash
            text: qsTr("Save stash")
            enabled: root.repository
                     ? root.repository.can(CEnums.Stash) && root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestStashSave()
        }

        Action {
            id: popStash
            text: qsTr("Pop stash")
            enabled: root.repository ? root.repository.can(CEnums.Stash) : false
            onTriggered: repositoryView.requestStashPop()
        }

        MenuSeparator { }

        Action {
            id: commit
            text: qsTr("&Commit")
            shortcut: "Ctrl+C"
            enabled: root.repository
                     ? root.repository.can(CEnums.Commit) && root.repository.hasCommitableFiles
                     : false
            onTriggered: repositoryView.requestCommit()
        }

        Action {
            id: amend
            text: qsTr("&Amend")
            shortcut: "Ctrl+A"
            enabled: root.repository
                     ? root.repository.can(CEnums.Amend) && root.repository.hasCommitableFiles && root.repository.commitCountAhead > 0
                     : false
            onTriggered: repositoryView.requestAmend()
        }

        MenuSeparator { }

        Action {
            text: qsTr("C&ontinue rebase")
            shortcut: "Ctrl+R"
            enabled: root.repository ? root.rebaseInProgress && root.repository.can(CEnums.ContinueRebase) : false
            onTriggered: repositoryView.requestContinueRebase()
        }

        Action {
            text: qsTr("Abor&t rebase")
            shortcut: "Ctrl+T"
            enabled: root.repository ? root.rebaseInProgress && root.repository.can(CEnums.AbortRebase) : false
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

        MenuSeparator { }

        MenuItem {
            text: qsTr("Show all")

            onClicked: {
                root.controller.showClean = true
                root.controller.showAdded = true
                root.controller.showModified = true
                root.controller.showRenamed = true
                root.controller.showDeleted = true
                root.controller.showMissing = true
                root.controller.showUntracked = true
                root.controller.showIgnored = true
            }
        }

        MenuSeparator { }

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
            text: qsTr("Show &renamed")
            checkable: true
            checked: root.controller.showRenamed
            onClicked: root.controller.showRenamed = !root.controller.showRenamed
        }

        MenuItem {
            text: qsTr("Show &deleted")
            checkable: true
            checked: root.controller.showDeleted
            onClicked: root.controller.showDeleted = !root.controller.showDeleted
        }

        MenuItem {
            text: qsTr("Show mi&ssing")
            checkable: true
            checked: root.controller.showMissing
            onClicked: root.controller.showMissing = !root.controller.showMissing
        }

        MenuItem {
            text: qsTr("Show &untracked")
            checkable: true
            checked: root.controller.showUntracked
            onClicked: root.controller.showUntracked = !root.controller.showUntracked
        }

        MenuItem {
            text: qsTr("Show &ignored")
            checkable: true
            checked: root.controller.showIgnored
            onClicked: root.controller.showIgnored = !root.controller.showIgnored
        }
    }

    Menu {
        title: qsTr("&Theme")

        MenuItem {
            text: qsTr("Dark")
            checkable: true
            checked: root.materialTheme === Material.Dark
            onClicked: root.requestDarkTheme()
        }

        MenuItem {
            text: qsTr("Light")
            checkable: true
            checked: root.materialTheme === Material.Light
            onClicked: root.requestLightTheme()
        }
    }

    Menu {
        title: qsTr("&Help")

        Action {
            text: qsTr("&About and shortcuts")
            onTriggered: root.requestHelp()
        }
    }
}
