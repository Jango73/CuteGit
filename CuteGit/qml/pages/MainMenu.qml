import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

MenuBar {
    id: root

    Material.elevation: 4

    property variant controller: null

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
                root.controller.fileModelProxy.refresh()
            }
        }

        MenuItem {
            text: qsTr("Show &clean")
            checkable: true
            checked: root.controller.fileModelProxy.showClean

            onClicked: {
                root.controller.fileModelProxy.showClean = !root.controller.fileModelProxy.showClean
            }
        }

        MenuItem {
            text: qsTr("Show &added")
            checkable: true
            checked: root.controller.fileModelProxy.showAdded

            onClicked: {
                root.controller.fileModelProxy.showAdded = !root.controller.fileModelProxy.showAdded
            }
        }

        MenuItem {
            text: qsTr("Show &modified")
            checkable: true
            checked: root.controller.fileModelProxy.showModified

            onClicked: {
                root.controller.fileModelProxy.showModified = !root.controller.fileModelProxy.showModified
            }
        }

        MenuItem {
            text: qsTr("Show &deleted")
            checkable: true
            checked: root.controller.fileModelProxy.showDeleted

            onClicked: {
                root.controller.fileModelProxy.showDeleted = !root.controller.fileModelProxy.showDeleted
            }
        }

        MenuItem {
            text: qsTr("Show &untracked")
            checkable: true
            checked: root.controller.fileModelProxy.showUntracked

            onClicked: {
                root.controller.fileModelProxy.showUntracked = !root.controller.fileModelProxy.showUntracked
            }
        }
    }

    Menu {
        title: qsTr("&Local")

        Action {
            text: qsTr("Stage &all")
            shortcut: "Ctrl+shift++"
            onTriggered: root.controller.fileModelProxy.stageAll()
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
            enabled: root.controller.fileModel.repositoryStatus === CFileModel.Rebase
            || root.controller.fileModel.repositoryStatus === CFileModel.InteractiveRebase
            onTriggered: root.requestContinueRebase()
        }
    }

    Menu {
        title: qsTr("Re&mote")

        Action {
            text: qsTr("&Pull...")
            shortcut: "Ctrl+L"
            onTriggered: root.controller.fileModelProxy.pull()
        }

        Action {
            text: qsTr("Pus&h...")
            shortcut: "Ctrl+P"
            onTriggered: root.controller.fileModelProxy.push()
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
