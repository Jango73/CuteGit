import QtQuick 2.12
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

TreeView {
    id: root
    enabled: visible

    property variant controller: null
    property bool mouseActive: true

    model: root.controller !== null ? root.controller.repository.treeFileModelProxy : undefined

    rootIndex: root.controller !== null ? root.controller.repository.treeFileModelProxy.rootPathIndex : undefined
    selection: root.selection
    selectionMode: 2
    // "None", "Single", "Extended", "Multi", "Contig."

    TableViewColumn {
        title: "Name"
        role: "fileName"
        width: root.width * 0.8
    }

    TableViewColumn {
        title: "Status"
        role: "status"
        width: root.width * 0.2
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
            property string status: root.model.statusForIndex(styleData.index)
            property string staged: root.model.stagedForIndex(styleData.index)

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
        for (var i = 0; i < root.model.rowCount(); i++) {
            var index = root.model.index(i, 0)

//                    if (!treeView.isExpanded(index)) {
                root.expand(index)
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
