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
        title: Const.localMenuText

        Action {
            text: Const.stageAllMenuText
            shortcut: "Ctrl+Shift++"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestStageAll()
        }

        Action {
            text: Const.unstageAllMenuText
            shortcut: "Ctrl+Shift+-"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestUnstageAll()
        }

        Action {
            id: stageSelection
            text: Const.stageSelectionMenuText
            shortcut: "Ctrl++"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestStageSelection()
        }

        Action {
            text: Const.unstageSelectionMenuText
            shortcut: "Ctrl+-"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestUnstageSelection()
        }

        MenuSeparator { }

        Action {
            id: revertSelection
            text: Const.revertSelectionMenuText
            shortcut: "Ctrl+Z"
            enabled: root.repository
                     ? root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestRevertSelection()
        }

        MenuSeparator { }

        Action {
            id: commit
            text: Const.commitMenuText
            shortcut: "Ctrl+C"
            enabled: root.repository
                     ? root.repository.can(CEnums.Commit) && root.repository.hasCommitableFiles
                     : false
            onTriggered: repositoryView.requestCommit()
        }

        Action {
            id: amend
            text: Const.amendMenuText
            shortcut: "Ctrl+A"
            enabled: root.repository
                     ? root.repository.can(CEnums.Amend) &&
                       (
                           (root.repository.repositoryStatus !== CEnums.NoMerge)
                           ||
                           (root.repository.repositoryStatus === CEnums.NoMerge && root.repository.hasCommitableFiles && root.repository.commitCountAhead > 0)
                           )
                     : false
            onTriggered: repositoryView.requestAmend()
        }

        MenuSeparator { }

        Action {
            text: Const.continueRebaseMenuText
            shortcut: "Ctrl+R"
            enabled: root.repository ? root.rebaseInProgress && root.repository.can(CEnums.ContinueRebase) : false
            onTriggered: repositoryView.requestContinueRebase()
        }

        Action {
            text: Const.abortRebaseMenuText
            shortcut: "Ctrl+T"
            enabled: root.repository ? root.rebaseInProgress && root.repository.can(CEnums.AbortRebase) : false
            onTriggered: repositoryView.requestAbortRebase()
        }
    }

    Menu {
        title: Const.stashAndPatchMenuText

        Action {
            id: saveStash
            text: Const.saveStashMenuText
            enabled: root.repository
                     ? root.repository.can(CEnums.Stash) && root.repository.hasModifiedFiles
                     : false
            onTriggered: repositoryView.requestStashSave()
        }

        Action {
            id: popStash
            text: Const.popStashMenuText
            enabled: root.repository ? root.repository.can(CEnums.Stash) : false
            onTriggered: repositoryView.requestStashPop()
        }

        MenuSeparator { }

        Action {
            id: applyPatch
            text: Const.applyPatchMenuText
            enabled: root.repository
                     ? root.repository.can(CEnums.ApplyPatch)
                     : false
            onTriggered: repositoryView.requestPatchApply()
        }
    }

    Menu {
        title: Const.viewMenuText

        Action {
            text: Const.refreshMenuText
            shortcut: "F5"
            onTriggered: repositoryView.requestRefresh()
        }

        MenuSeparator { }

        MenuItem {
            text: Const.showAllMenuText

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
            text: Const.showCleanMenuText
            checkable: true
            checked: root.controller.showClean
            onClicked: root.controller.showClean = !root.controller.showClean
        }

        MenuItem {
            text: Const.showAddedMenuText
            checkable: true
            checked: root.controller.showAdded
            onClicked: root.controller.showAdded = !root.controller.showAdded
        }

        MenuItem {
            text: Const.showModifiedMenuText
            checkable: true
            checked: root.controller.showModified
            onClicked: root.controller.showModified = !root.controller.showModified
        }

        MenuItem {
            text: Const.showRenamedMenuText
            checkable: true
            checked: root.controller.showRenamed
            onClicked: root.controller.showRenamed = !root.controller.showRenamed
        }

        MenuItem {
            text: Const.showDeletedMenuText
            checkable: true
            checked: root.controller.showDeleted
            onClicked: root.controller.showDeleted = !root.controller.showDeleted
        }

        MenuItem {
            text: Const.showMissingMenuText
            checkable: true
            checked: root.controller.showMissing
            onClicked: root.controller.showMissing = !root.controller.showMissing
        }

        MenuItem {
            text: Const.showUntrackedMenuText
            checkable: true
            checked: root.controller.showUntracked
            onClicked: root.controller.showUntracked = !root.controller.showUntracked
        }

        MenuItem {
            text: Const.showIgnoredMenuText
            checkable: true
            checked: root.controller.showIgnored
            onClicked: root.controller.showIgnored = !root.controller.showIgnored
        }
    }

    Menu {
        title: Const.optionsMenuText

        Action {
            text: Const.aboutMenuText
            onTriggered: root.requestHelp()
        }

        Menu {
            title: Const.themeMenuText

            MenuItem {
                text: Const.darkThemeMenuText
                checkable: true
                checked: root.materialTheme === Material.Dark
                onClicked: root.requestDarkTheme()
            }

            MenuItem {
                text: Const.lightThemeMenuText
                checkable: true
                checked: root.materialTheme === Material.Light
                onClicked: root.requestLightTheme()
            }
        }

        Menu {
            id: language
            title: Const.languageMenuText

            Instantiator {
                model: root.controller.langModel

                MenuItem {
                    text: model.display
                    checkable: true
                    checked: root.controller.language === model.display
                    onClicked: root.controller.language = model.display
                }

                onObjectAdded: language.insertItem(index, object)
                onObjectRemoved: language.removeItem(object)
            }
        }
    }
}
