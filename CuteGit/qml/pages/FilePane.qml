import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"
import "../views"
import "../popups"

TitlePane {
    id: root

    property variant repository: null
    property variant treeSelection: null
    property variant flatSelection: null
    property bool filesAsTree: false

    title: Const.filesText

    content: Item {
        anchors.fill: parent

        StandardLabel {
            anchors.fill: parent
            text: Const.listEmptyText
            visible: root.repository === null | listView.count === 0
        }

        FlatFileView {
            id: listView
            anchors.fill: parent
            visible: !root.filesAsTree
            enabled: visible
            activeFocusOnTab: true

            repository: root.repository
            selection: root.flatSelection
        }

        Item {
            anchors.fill: parent
            visible: root.filesAsTree

            ToolBar {
                id: toolbar
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right

                Row {
                    ToolButton { text: Const.expandAllText; action: Action { onTriggered: treeView.expandAll() } }
                    ToolButton { text: Const.collapseAllText; action: Action { onTriggered: treeView.collapseAll() } }
                }
            }

            TreeFileView {
                id: treeView
                anchors.top: toolbar.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                visible: root.filesAsTree

                repository: root.repository
                selection: root.treeSelection
            }
        }
    }

    function getSelectedFiles() {
        var files
        if (filesAsTree) {
            files = treeView.model.selectionToFullNameList(treeSelection.selectedIndexes)
        } else {
            files = listView.model.selectionToFullNameList(flatSelection.selectedIndexes)
        }
        return files
    }
}
