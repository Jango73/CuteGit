import QtQuick 2.2
import QtQuick.Controls 1.5
import QtQuick.Controls.Material 2.12
import QtQml.Models 2.2
import "../components"

Item {
    id: root

    ItemSelectionModel {
        id: sel
        model: fileSystemModel
    }

    TreeView {
        id: view
        anchors.fill: parent
        model: fileSystemModel
        rootIndex: rootPathIndex
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
            width: 70
        }

        TableViewColumn {
            title: "State"
            role: "state"
            resizable: true
            width: 100
        }
    }
}
