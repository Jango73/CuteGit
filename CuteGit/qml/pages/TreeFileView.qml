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
    backgroundVisible: false
    headerVisible: false
    selection: root.selection
    selectionMode: 2
    // "None", "Single", "Extended", "Multi", "Contig."

    TableViewColumn {
        title: "Name"
        role: "fileName"
    }

    rowDelegate: rowDlg
    itemDelegate: itemDlg

    Component {
        id: rowDlg

        Rectangle {
            height: Const.treeElementHeight
            color: Material.background
        }
    }

    Component {
        id: itemDlg

        Rectangle {
            height: Const.treeElementHeight
            color: Material.background

            Rectangle {
                anchors.fill: parent
                color: if (model.staged === "X") Const.fileStagedColor
                       else if (model.status === "*") Const.fileModifiedColor
                       else if (model.status === "=") Const.fileRenamedColor
                       else if (model.status === "+") Const.fileAddedColor
                       else if (model.status === "-") Const.fileDeletedColor
                       else Const.transparent
            }

            Selection {
                id: selectionIndicator
                targetWidth: fileNameText.width
                targetHeight: fileNameText.height
                anchors.centerIn: fileNameText
                borderOnly: true
                visible: styleData.selected

                FocusIndicator {
                    anchors.fill: parent
                    visible: root.activeFocus
                }
            }

            ElideText {
                id: statusText
                width: Const.elementHeight
                height: parent.height
                text: model.status
            }

            ElideText {
                id: fileNameText
                anchors.left: statusText.right
                anchors.right: parent.right
                height: parent.height
                text: model.fileName
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Space) {
            var currentFileFullName = model.fullNameForIndex(currentIndex)
            root.controller.repository.toggleStaged(currentFileFullName)
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
}
