import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import "../components"

Item {
    id: root

    property variant fileModel: null
    property variant repositoryModel: null

    MenuBar {
        id: menu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
//        height: Const.mainFontSize

//        Font.family: Const.mainFontFamily
//        Font.pixelSize: Const.mainFontSize

        Material.elevation: 6

        Menu {
            title: qsTr("&Repository")
            Action { text: qsTr("&Add...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }

        Menu {
            title: qsTr("&Local")
            Action { text: qsTr("&Commit...") }
        }

        Menu {
            title: qsTr("&Remote")
            Action { text: qsTr("&Pull...") }
            Action { text: qsTr("&Push...") }
        }
    }

    Item {
        id: container
        anchors.top: menu.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Pane {
            id: repositoryView
            anchors.top: parent.top
            anchors.bottom: graphView.top
            width: parent.width * 0.15
            anchors.margins: Const.paneMargins

            Material.elevation: Const.paneElevation

            ListView {
                anchors.fill: parent
                clip: true
                interactive: true

                model: root.repositoryModel

                delegate: StandardText {
                    width: parent.width
                    height: Const.elementHeight
                    text: display.split("/").slice(-1)[0]
                }
            }
        }

        Item {
            id: centralPart
            anchors.top: parent.top
            anchors.bottom: graphView.top
            anchors.left: repositoryView.right
            anchors.right: parent.right

            FileView {
                id: fileView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width * 0.5
                anchors.margins: Const.paneMargins

                fileModel: root.fileModel
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

                model: root.fileModel != null ? root.fileModel.graphModel : undefined

                delegate: StandardText {
                    width: parent.width
                    height: Const.elementHeight
                    text: display
                }
            }
        }
    }
}
