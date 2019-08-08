import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import "../components"
import "../views"
import "../popups"

Pane {
    id: root

    property variant repository: null

    signal requestMenu(var commitId, var message)

    TabBar {
        id: tabBar
        anchors.top: parent.top

        TabButton {
            id: branchLogTab
            width: implicitWidth
            text: Const.branchLogText
        }

        TabButton {
            id: graphTab
            width: implicitWidth
            text: Const.graphText
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

        Item {
            StandardLabel {
                anchors.fill: parent
                text: Const.listEmptyText
                visible: root.repository === null | logView.count === 0
            }

            LogView {
                id: logView
                anchors.fill: parent

                model: root.repository !== null ? root.repository.logModel : undefined

                onItemRightClicked: {
                    root.requestMenu(commitId, message)
                }
            }
        }

        Item {
            GraphView {
                id: graphView
                anchors.fill: parent

                model: root.repository !== null ? root.repository.graphModel : undefined
            }
        }
    }

    function activateBranchLogView() {
        tabBar.currentIndex = 0
        logView.forceActiveFocus()
    }

    function activateGraphView() {
        tabBar.currentIndex = 1
        graphView.forceActiveFocus()
    }
}
