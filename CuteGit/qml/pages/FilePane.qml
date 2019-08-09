import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"
import "../views"
import "../popups"

Pane {
    id: root
    padding: Const.mainPadding

    Material.elevation: Const.paneElevation

    property variant repository: null
    property variant treeSelection: null
    property variant flatSelection: null

    TabBar {
        id: tabBar
        anchors.top: parent.top

        TabButton {
            width: implicitWidth
            text: Const.flatText
        }

        TabButton {
            width: implicitWidth
            text: Const.treeText
        }
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        interactive: false
        currentIndex: tabBar.currentIndex
        clip: true

        Item {
            StandardLabel {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: Const.allFilesCleanText
                visible: root.repository === null | flatFileView.count === 0
            }

            FlatFileView {
                id: flatFileView
                anchors.fill: parent
                visible: !root.filesAsTree
                enabled: visible

                repository: root.repository
                selection: root.flatSelection
            }
        }

        Item {
            /*
                TODO: make expand and collapse all functions work
                StandardToolBar {
                    id: toolbar
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Row {
                        StandardToolButton { text: Const.expandAllText; action: Action { onTriggered: treeView.expandAll() } }
                        StandardToolButton { text: Const.collapseAllText; action: Action { onTriggered: treeView.collapseAll() } }
                    }
                }
                */

            TreeFileView {
                id: treeFileView
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                repository: root.repository
                selection: root.treeSelection
            }
        }
    }

    function getSelectedFiles() {
        var files
        if (tabBar.currentIndex === 0) {
            files = flatFileView.model.selectionToFullNameList(flatSelection.selectedIndexes)
        } else {
            files = treeFileView.model.selectionToFullNameList(treeSelection.selectedIndexes)
        }
        return files
    }

    function activateFlatFileView() {
        tabBar.currentIndex = 0
        flatFileView.forceActiveFocus()
    }
}
