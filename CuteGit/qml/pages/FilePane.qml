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

    Item {
        id: toolbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: expandAllButton.height * 1.1

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
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible: true

        model: root.controller.flatFileModel

        delegate: Item {
            width: parent.width
            height: Const.treeElementHeight

            property string status: treeView.model.statusForIndex(styleData.index)
            property string staged: treeView.model.stagedForIndex(styleData.index)

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.AllButtons
                onClicked: listView.currentIndex = index
            }

            Rectangle {
                anchors.fill: parent
                color: if (model.staged === "O") Const.fileStagedColor
                       else if (model.status === "*") Const.fileModifiedColor
                       else if (model.status === "+") Const.fileAddedColor
                       else if (model.status === "-") Const.fileDeletedColor
                       else Const.transparent
            }

            Selection {
                id: selection
                targetWidth: listViewText.width
                targetHeight: listViewText.height
                anchors.centerIn: listViewText
                borderOnly: true
                visible: index === listView.currentIndex
            }

            StandardText {
                id: listViewText
                anchors.fill: parent
                height: Const.treeElementHeight
                wrapMode: Text.NoWrap
                elide: Text.ElideRight
                color: Material.foreground
                text: model.fileName
            }
        }
    }

    QQC15.TreeView {
        id: treeView
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        focus: true
        visible: false

        model: root.controller.fileModelProxy
        rootIndex: root.controller.fileModelProxy !== null ? root.controller.fileModelProxy.rootPathIndex : undefined
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

                StandardText {
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
                    color: if (staged === "O") Const.fileStagedColor
                           else if (status === "*") Const.fileModifiedColor
                           else if (status === "+") Const.fileAddedColor
                           else if (status === "-") Const.fileDeletedColor
                           else Const.transparent
                }

                Selection {
                    id: selection
                    targetWidth: text.width
                    targetHeight: text.height
                    anchors.centerIn: text
                    visible: styleData.selected && styleData.value !== ""
                    borderOnly: true
                }

                StandardText {
                    id: text
                    height: Const.treeElementHeight
                    text: styleData.value
                    elide: styleData.elideMode
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
