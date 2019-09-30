import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

StandardListView {
    id: root

    signal itemRightClicked(var operation, var text)

    delegate: StandardListViewItem {
        width: parent.width
        listView: root
        primaryText: model.text
        primaryTextColor: model.operation === CEnums.DiffFileName ? Material.background : Material.foreground
        selectionShown: false

        background: Rectangle {
            anchors.fill: parent
            color: if (model.operation === CEnums.DiffAdd) Const.fileStagedColor
                   else if (model.operation === CEnums.DiffDelete) Const.fileModifiedColor
                   else if (model.operation === CEnums.DiffFileName) Material.accent
                   else Const.transparent
        }

        onClicked: {
            if (mouse.button === Qt.RightButton) {
                root.itemRightClicked(model.operation, model.text)
            }
        }
    }
}
