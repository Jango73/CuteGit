import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

TitlePane {
    id: root

    property variant controller: null
    property variant selection: null
    property bool filesAsTree: false

    title: Const.filesText

    content: Item {
        anchors.fill: parent

        FlatFileView {
            id: listView
            anchors.fill: parent
            visible: !root.filesAsTree
            enabled: visible
            activeFocusOnTab: true

            controller: root.controller
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

                controller: root.controller
                selection: root.selection
            }
        }
    }

    function getSelectedFiles() {
        var files
        if (filesAsTree) {
            files = treeView.model.selectionToFullNameList(selection.selectedIndexes)
        } else {
        }
        return files
    }
}
