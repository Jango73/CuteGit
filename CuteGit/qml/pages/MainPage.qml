import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtQml.Models 2.2
import Qt.labs.platform 1.1
import "../components"

Item {
    id: root

    property variant controller: null

    MenuBar {
        id: menu
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.top: parent.top

        Material.elevation: 4

        Menu {
            title: qsTr("&Repository")

            Action { text: qsTr("&Add...") }

            MenuSeparator { }

            Action { text: qsTr("&Quit") }
        }

        Menu {
            title: qsTr("&Files")

            MenuItem {
                text: qsTr("View unchanged");
                checkable: true
                checked: true
            }

            MenuItem {
                text: qsTr("View added");
                checkable: true
            }

            MenuItem {
                text: qsTr("View modified");
                checkable: true
            }

            MenuItem {
                text: qsTr("View deleted");
                checkable: true
            }
        }

        Menu {
            title: qsTr("&Local")

            Action {
                text: qsTr("&Stage")
                shortcut: "Ctrl++"

                onTriggered: {
                    root.controller.fileModel.stageSelection(fileSelection.selectedIndexes)
                }
            }

            Action {
                text: qsTr("&Unstage")
                shortcut: "Ctrl+-"

                onTriggered: {
                    root.controller.fileModel.unstageSelection(fileSelection.selectedIndexes)
                }
            }

            MenuItem { text: qsTr("&Commit...") }
        }

        Menu {
            title: qsTr("Re&mote")

            MenuItem { text: qsTr("&Pull...") }

            MenuItem { text: qsTr("Pus&h...") }
        }
    }

    Item {
        id: container
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        RepoPane {
            id: repositoryView
            anchors.top: parent.top
            anchors.bottom: outputView.top
            anchors.right: parent.right
            width: parent.width * 0.15
            anchors.margins: Const.paneMargins

            controller: root.controller
        }

        Item {
            id: centralPart
            anchors.top: parent.top
            anchors.bottom: outputView.top
            anchors.left: parent.left
            anchors.right: repositoryView.left

            ItemSelectionModel {
                id: fileSelection
                model: root.controller.fileModel
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

            Pane {
                id: toolView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: fileView.right
                anchors.right: parent.right
                anchors.margins: Const.paneMargins

                Material.elevation: Const.paneElevation

                StandardText {
                    anchors.fill: parent
                    text: "TOOLS"
                }
            }
        }

        Pane {
            id: outputView
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: graphView.top
            height: parent.height * 0.05
            anchors.margins: Const.paneMargins

            Material.elevation: Const.paneElevation

            ListView {
                anchors.fill: parent
                clip: true
                interactive: true

                model: root.controller.commandOutputModel

                delegate: StandardText {
                    width: parent.width
                    height: Const.elementHeight
                    text: display
                }
            }
        }

        Pane {
            id: graphView
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height * 0.33
            anchors.margins: Const.paneMargins

            Material.elevation: Const.paneElevation

            ListView {
                anchors.fill: parent
                clip: true
                interactive: true

                model: root.controller.fileModel !== null ? root.controller.fileModel.graphModel : undefined

                delegate: StandardText {
                    width: parent.width
                    height: Const.elementHeight
                    text: display
                }
            }
        }
    }
}
