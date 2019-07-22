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

    signal requestOpenRepository()
    signal requestStageSelection()
    signal requestUnstageSelection()
    signal requestRevertSelection()
    signal requestCommit()
    signal requestAmend()
    signal requestContinueRebase()
    signal requestShortcuts()

    Menu {
        title: qsTr("&Repository")

        Action {
            text: qsTr("&Open...")
            shortcut: "Ctrl+O"
            onTriggered: root.requestOpenRepository()
        }

        Action {
            text: qsTr("&Remove...")
        }

        MenuSeparator { }

        Action {
            text: qsTr("&Quit")
            shortcut: "Ctrl+Q"
            onTriggered: root.controller.quit()
        }
    }

    Menu {
        title: qsTr("&View")

        Action {
            text: qsTr("&Refresh")
            shortcut: "F5"

            onTriggered: {
                root.controller.treeFileModelProxy.refresh()
            }
        }

        MenuItem {
            text: qsTr("Files as a tree")
            checkable: true
            checked: root.filesAsTree

            onClicked: {
                root.filesAsTree = !root.filesAsTree
            }
        }

        MenuItem {
            text: qsTr("Show &clean")
            checkable: true
            checked: root.controller.showClean

            onClicked: {
                root.controller.showClean = !root.controller.showClean
            }
        }

        MenuItem {
            text: qsTr("Show &added")
            checkable: true
            checked: root.controller.showAdded

            onClicked: {
                root.controller.showAdded = !root.controller.showAdded
            }
        }

        MenuItem {
            text: qsTr("Show &modified")
            checkable: true
            checked: root.controller.showModified

            onClicked: {
                root.controller.showModified = !root.controller.showModified
            }
        }

        MenuItem {
            text: qsTr("Show &deleted")
            checkable: true
            checked: root.controller.showDeleted

            onClicked: {
                root.controller.showDeleted = !root.controller.showDeleted
            }
        }

        MenuItem {
            text: qsTr("Show &untracked")
            checkable: true
            checked: root.controller.showUntracked

            onClicked: {
                root.controller.showUntracked = !root.controller.showUntracked
            }
        }
    }

    Menu {
        title: qsTr("&Local")

        Action {
            text: qsTr("Stage &all")
            shortcut: "Ctrl+shift++"
            onTriggered: root.controller.treeFileModelProxy.stageAll()
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
            text: qsTr("&Commit...")
            shortcut: "Ctrl+C"
            onTriggered: root.requestCommit()
        }

        Action {
            text: qsTr("&Amend...")
            shortcut: "Ctrl+A"
            onTriggered: root.requestAmend()
        }

        Action {
            text: qsTr("&Continue rebase")
            shortcut: "Ctrl+R"
            enabled: root.controller.treeFileModel.repositoryStatus === CTreeFileModel.Rebase
            || root.controller.treeFileModel.repositoryStatus === CTreeFileModel.InteractiveRebase
            onTriggered: root.requestContinueRebase()
        }
    }

    Menu {
        title: qsTr("Re&mote")

        Action {
            text: qsTr("&Pull...")
            shortcut: "Ctrl+L"
            onTriggered: root.controller.treeFileModelProxy.pull()
        }

        Action {
            text: qsTr("Pus&h...")
            shortcut: "Ctrl+P"
            onTriggered: root.controller.treeFileModelProxy.push()
        }
    }

    Menu {
        title: qsTr("&Help")

        Action {
            text: qsTr("&Shortcuts...")
            onTriggered: root.requestShortcuts()
        }
    }
}
