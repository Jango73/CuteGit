import QtQuick 2.12
import QtQuick.Controls 1.5
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import QtQml.Models 2.2
import CuteGit 1.0
import "../components"

Pane {
    id: root
    padding: 2

    Material.elevation: Const.paneElevation

    property variant fileModel: null

    ItemSelectionModel {
        id: sel
        model: root.fileModel
    }

    TreeView {
        id: view
        anchors.fill: parent

        model: root.fileModel
        rootIndex: root.fileModel !== null ? root.fileModel.rootPathIndex : undefined
        selection: sel

        TableViewColumn {
            title: "Status"
            role: "status"
            // resizable: true
            width: view.width * 0.1
        }

        TableViewColumn {
            title: "Name"
            role: "fileName"
            // resizable: true
            width: view.width * 0.7
        }

        TableViewColumn {
            title: "Size"
            role: "size"
            // resizable: true
            horizontalAlignment : Text.AlignRight
            width: view.width * 0.2
        }

//        style: TreeViewStyle {
//            rowDelegate: Rectangle {
//                color: "blue"
//            }

//            itemDelegate: StandardText {
//                width: 50
//                height: 30
//                text: styleData.value
//            }
//        }
    }
}
