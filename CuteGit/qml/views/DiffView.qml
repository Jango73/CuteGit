import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

Pane {
    id: root
    padding: Const.mainPadding

    property variant repository: null

    StandardLabel {
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Const.listEmptyText
        visible: root.repository === null | list.count === 0
    }

    StandardListView {
        id: list
        anchors.fill: parent
        model: root.repository !== null ? root.repository.fileDiffModel : undefined

        delegate: StandardListViewItem {
            width: parent.width
            listView: parent
            primaryText: model.text
            primaryTextColor: model.operation === CEnums.DiffFileName ? Material.background : Material.foreground
            selectionShown: false
            focusShown: false

            background: Rectangle {
                anchors.fill: parent
                color: if (model.operation === CEnums.DiffAdd) Const.fileStagedColor
                       else if (model.operation === CEnums.DiffDelete) Const.fileModifiedColor
                       else if (model.operation === CEnums.DiffFileName) Material.accent
                       else Const.transparent
            }
        }
    }
}
