import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import "../components"

Item {
    id: root

    property variant model: null

    MenuBar {
        id: menu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        //        height: 100

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

    FileView {
        id: fileView
        anchors.top: menu.bottom
        anchors.bottom: logView.top
        anchors.left: parent.left
        anchors.right: toolView.left
        model: root.model
    }

    Pane {
        id: toolView
        anchors.top: menu.bottom
        anchors.bottom: logView.top
        anchors.right: parent.right
        width: parent.width * 0.5
    }

    Pane {
        id: logView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height * 0.33
    }
}
