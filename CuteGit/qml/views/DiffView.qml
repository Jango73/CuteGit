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

        delegate: Item {
            width: parent.width
            height: Const.listViewItemHeight

            Rectangle {
                anchors.fill: parent
                color: if (operation === CEnums.DiffAdd) Const.fileStagedColor
                       else if (operation === CEnums.DiffDelete) Const.fileModifiedColor
                       else if (operation === CEnums.DiffFileName) Material.accent
                       else Const.transparent
            }

            StandardText {
                anchors.fill: parent
                text: model.text
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
                color: operation === CEnums.DiffFileName ? Material.background : Material.foreground
            }
        }
    }
}
