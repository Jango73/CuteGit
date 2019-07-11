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

    Material.elevation: 4

    property variant model: null

    ItemSelectionModel {
        id: sel
        model: root.model
    }

    TreeView {
        id: view
        anchors.fill: parent

        model: root.model
        rootIndex: model.rootPathIndex
        selection: sel

        TableViewColumn {
            title: "Name"
            role: "fileName"
            resizable: true
        }

        TableViewColumn {
            title: "Size"
            role: "size"
            resizable: true
            horizontalAlignment : Text.AlignRight
            width: 80
        }

        TableViewColumn {
            title: "State"
            role: "state"
            resizable: true
            width: 100
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
