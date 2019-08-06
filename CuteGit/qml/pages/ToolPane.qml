import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"
import "../views"
import "../popups"

Pane {
    id: root
    padding: Const.mainPadding

    Material.elevation: Const.paneElevation

    property variant repository: null

    TabBar {
        id: tabBar
        anchors.top: parent.top

        TabButton {
            width: implicitWidth
            text: qsTr("Diff")
        }

        TabButton {
            width: implicitWidth
            text: qsTr("Log")
        }
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        interactive: false
        clip: true
        currentIndex: tabBar.currentIndex

        DiffView {
            repository: root.repository
        }

        FileLogPane {
            repository: root.repository
        }
    }
}
