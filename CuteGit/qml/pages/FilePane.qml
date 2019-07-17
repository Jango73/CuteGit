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
                    view.expandAll()
                }
            }

            StandardButton {
                id: collapseAllButton
                Layout.alignment: Qt.AlignCenter
                text: Const.collapseAllText

                onClicked: {
                    view.collapseAll()
                }
            }
        }
    }

    QQC15.TreeView {
        id: view
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: root.controller.fileModelProxy
        rootIndex: root.controller.fileModelProxy !== null ? root.controller.fileModelProxy.rootPathIndex : undefined
        selection: root.selection
        selectionMode: 2
        // "None", "Single", "Extended", "Multi", "Contig."

        QQC15.TableViewColumn {
            title: "Name"
            role: "fileName"
            width: view.width * 0.7
        }

        QQC15.TableViewColumn {
            title: "Status"
            role: "status"
            width: view.width * 0.2
        }

        QQC15.TableViewColumn {
            title: "Staged"
            role: "staged"
            width: view.width * 0.1
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
                property string status: view.model.statusForIndex(styleData.index)
                property string staged: view.model.stagedForIndex(styleData.index)

                Rectangle {
                    anchors.fill: parent
                    color: if (staged === "O") "#080"
                           else if (status === "*") "#800"
                           else if (status === "+") "#008"
                           else "transparent"
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
            for (var i = 0; i < view.model.rowCount(); i++) {
                var index = view.model.index(i, 0)

                if (!view.isExpanded(index)) {
                    view.expand(index)
                }
            }
        }

        function collapseAll() {
        }
    }
}
