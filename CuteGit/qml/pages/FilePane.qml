import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.5 as QQC15
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

Pane {
    id: root
    padding: 2
    Material.elevation: Const.paneElevation

    property variant controller: null
    property variant selection: null
    property bool filesAsTree: false

    Item {
        id: toolbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: expandAllButton.height * 1.1
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

    ListView {
        id: listView
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: !root.filesAsTree
        enabled: visible
        activeFocusOnTab: true

        model: root.controller.flatFileModel

        onCurrentIndexChanged: root.controller.flatFileModel.handleCurrentIndex(listView.model.index(currentIndex, 0))

        delegate: Item {
            width: parent.width
            height: Const.treeElementHeight + Const.mainPadding * 0.25

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.AllButtons
                onClicked: {
                    listView.currentIndex = index
                }
            }

            Rectangle {
                anchors.fill: parent
                color: if (model.staged === "X") Const.fileStagedColor
                       else if (model.status === "*") Const.fileModifiedColor
                       else if (model.status === "=") Const.fileRenamedColor
                       else if (model.status === "+") Const.fileAddedColor
                       else if (model.status === "-") Const.fileDeletedColor
                       else Const.transparent
            }

            Item {
                id: listViewFileName
                width: parent.width * 0.4
                height: parent.height

                Selection {
                    id: listSelection
                    targetWidth: listViewFileNameText.width
                    targetHeight: listViewFileNameText.height
                    anchors.centerIn: listViewFileNameText
                    borderOnly: true
                    visible: index === listView.currentIndex

                    FocusIndicator {
                        anchors.fill: parent
                        visible: listView.activeFocus
                    }
                }

                ElideText {
                    id: listViewFileNameText
                    width: parent.width - Const.smallPadding
                    anchors.centerIn: parent
                    color: Material.foreground
                    text: model.fileName
                }
            }

            Item {
                id: listViewRelativeName
                width: parent.width * 0.6
                height: parent.height
                anchors.left: listViewFileName.right

                ElideText {
                    id: listViewRelativeNameText
                    width: parent.width - Const.smallPadding
                    anchors.centerIn: parent
                    color: Material.foreground
                    text: model.relativeName
                }
            }
        }
    }

    QQC15.TreeView {
        id: treeView
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: root.filesAsTree
        enabled: visible

        model: root.controller.treeFileModelProxy
        rootIndex: root.controller.treeFileModelProxy !== null ? root.controller.treeFileModelProxy.rootPathIndex : undefined
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

                if (!treeView.isExpanded(index)) {
                    treeView.expand(index)
                }
            }
        }

        function collapseAll() {
        }
    }
}
