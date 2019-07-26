import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.5 as QQC15
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
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

        Item {
            id: toolbar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: expandAllButton.height + Const.smallPadding
            visible: root.filesAsTree

            RowLayout {
                id: buttons
                anchors.fill: parent

                StandardButton {
                    id: expandAllButton
                    Layout.alignment: Qt.AlignCenter
                    text: Const.expandAllText

                    onClicked: {
                        treeView.expandAll()
                    }
                }

                StandardButton {
                    id: collapseAllButton
                    Layout.alignment: Qt.AlignCenter
                    text: Const.collapseAllText

                    onClicked: {
                        treeView.collapseAll()
                    }
                }
            }
        }

        FlatFileView {
            id: listView
            anchors.fill: parent
            visible: !root.filesAsTree
            enabled: visible
            activeFocusOnTab: true

            controller: root.controller
        }

        QQC15.TreeView {
            id: treeView
            anchors.top: toolbar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            visible: root.filesAsTree
            enabled: visible

            model: root.controller !== null ? root.controller.repository.treeFileModelProxy : undefined
            rootIndex: root.controller !== null ? root.controller.repository.treeFileModelProxy.rootPathIndex : undefined
            selection: root.selection
            selectionMode: 2
            // "None", "Single", "Extended", "Multi", "Contig."

            QQC15.TableViewColumn {
                title: "Name"
                role: "fileName"
                width: treeView.width * 0.8
            }

            QQC15.TableViewColumn {
                title: "Status"
                role: "status"
                width: treeView.width * 0.2
            }

            style: TreeViewStyle {
                headerDelegate: Item {
                    height: Const.treeElementHeight

                    Rectangle {
                        anchors.fill: parent
                        color: Material.background
                    }

                    ElideText {
                        text: styleData.value
                    }
                }

                rowDelegate: Rectangle {
                    color: Material.background
                    height: Const.treeElementHeight
                }

                itemDelegate: Item {
                    property string status: treeView.model.statusForIndex(styleData.index)
                    property string staged: treeView.model.stagedForIndex(styleData.index)

                    Rectangle {
                        anchors.fill: parent
                        color: if (staged === "X") Const.fileStagedColor
                               else if (status === "*") Const.fileModifiedColor
                               else if (status === "=") Const.fileRenamedColor
                               else if (status === "+") Const.fileAddedColor
                               else if (status === "-") Const.fileDeletedColor
                               else Const.transparent
                    }

                    Selection {
                        id: treeSelection
                        targetWidth: treeText.width
                        targetHeight: treeText.height
                        anchors.centerIn: treeText
                        visible: styleData.selected && styleData.value !== ""
                        borderOnly: true
                    }

                    ElideText {
                        id: treeText
                        height: Const.treeElementHeight
                        text: styleData.value
                        color: Material.foreground
                    }
                }
            }

            function expandAll() {
                for (var i = 0; i < treeView.model.rowCount(); i++) {
                    var index = treeView.model.index(i, 0)

//                    if (!treeView.isExpanded(index)) {
                        treeView.expand(index)
//                    }
                }
            }

            function collapseAll() {
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Space) {
                    // root.controller.repository.toggleStaged(currentItem.fullName)
                }
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
