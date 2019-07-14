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

            MenuItem {
                text: qsTr("View unchanged")
                checkable: true
                checked: true
            }

            MenuItem {
                text: qsTr("View added")
                checkable: true
                checked: true
            }

            MenuItem {
                text: qsTr("View modified")
                checkable: true
                checked: true
            }

            MenuItem {
                text: qsTr("View deleted")
                checkable: true
                checked: true
            }
        }

        Menu {
            title: qsTr("&Local")

            Action {
                text: qsTr("&Stage")
                shortcut: "Ctrl++"

                onTriggered: {
                    root.controller.fileModelProxy.stageSelection(fileSelection.selectedIndexes)
                }
            }

            Action {
                text: qsTr("&Unstage")
                shortcut: "Ctrl+-"

                onTriggered: {
                    root.controller.fileModelProxy.unstageSelection(fileSelection.selectedIndexes)
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
            }

            Action {
                text: qsTr("Pus&h...")
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

//                controller: root.controller
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

            Pane {
                id: outputView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: graphView.right
                anchors.right: parent.right
                anchors.margins: Const.paneMargins

                Material.elevation: Const.paneElevation

                StandardStringListView {
                    anchors.fill: parent
                    autoScrollToEnd: true
                    model: root.controller.commandOutputModel
                }
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

    QLP.FolderDialog {
        id: folderDialog

        onAccepted: {
            root.controller.repositoryPath = folder
        }
    }
}
