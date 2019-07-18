import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2
import Qt.labs.platform 1.1 as QLP
import Qt.labs.folderlistmodel 2.1
import "../components"

Item {
    id: root

    property variant controller: null

    MenuBar {
        id: menu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Material.elevation: 4

        Menu {
            title: qsTr("&Repository")

            Action {
                text: qsTr("&Open...")
                shortcut: "Ctrl+O"

                onTriggered: {
                    folderDialog.open()
                }
            }

            Action {
                text: qsTr("&Remove...")
            }

            MenuSeparator { }

            Action {
                text: qsTr("&Quit")
                shortcut: "Ctrl+Q"

                onTriggered: {
                    root.controller.quit()
                }
            }
        }

        Menu {
            title: qsTr("&Files")

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

                onTriggered: {
                    root.controller.fileModelProxy.stageSelection(fileSelection.selectedIndexes)
                }
            }

            Action {
                text: qsTr("&Stage selection")
                shortcut: "Ctrl++"

                onTriggered: {
                    root.controller.fileModelProxy.stageSelection(fileSelection.selectedIndexes)
                }
            }

            Action {
                text: qsTr("&Unstage selection")
                shortcut: "Ctrl+-"

                onTriggered: {
                    root.controller.fileModelProxy.unstageSelection(fileSelection.selectedIndexes)
                }
            }

            Action {
                text: qsTr("&Revert selection")
                shortcut: "Ctrl+Z"

                onTriggered: {
                    root.controller.fileModelProxy.revertSelection(fileSelection.selectedIndexes)
                }
            }

            Action {
                text: qsTr("&Commit...")
                shortcut: "Ctrl+C"

                onTriggered: {
                    commit.open()
                }
            }
        }

        Menu {
            title: qsTr("Re&mote")

            Action {
                text: qsTr("&Pull...")
                shortcut: "Ctrl+L"

                onTriggered: {
                    root.controller.fileModelProxy.pull()
                }
            }

            Action {
                text: qsTr("Pus&h...")
                shortcut: "Ctrl+P"

                onTriggered: {
                    root.controller.fileModelProxy.push()
                }
            }
        }

        Menu {
            title: qsTr("&Help")

            Action {
                text: qsTr("&Shortcuts...")

                onTriggered: {
                    shortcuts.open()
                }
            }
        }
    }

    Item {
        id: container
        anchors.top: menu.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        RepoPane {
            id: repositoryView
            anchors.top: parent.top
            anchors.bottom: bottomPart.top
            anchors.right: parent.right
            width: parent.width * 0.15
            anchors.margins: Const.paneMargins

            controller: root.controller
        }

        Item {
            id: centralPart
            anchors.top: parent.top
            anchors.bottom: bottomPart.top
            anchors.left: parent.left
            anchors.right: repositoryView.left

            ItemSelectionModel {
                id: fileSelection
                model: root.controller.fileModelProxy

                onCurrentIndexChanged: {
                    root.controller.fileModelProxy.handleCurrentIndex(currentIndex)
                }
            }

            FilePane {
                id: fileView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width * 0.5
                anchors.margins: Const.paneMargins

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

            Pane {
                id: graphView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width * 0.5
                anchors.margins: Const.paneMargins

                Material.elevation: Const.paneElevation

                StandardStringListView {
                    anchors.fill: parent
                    model: root.controller.fileModel !== null ? root.controller.fileModel.graphModel : undefined
                }
            }

            OutputPane {
                id: outputView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: graphView.right
                anchors.right: parent.right
                anchors.margins: Const.paneMargins

                controller: root.controller
            }
        }
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
