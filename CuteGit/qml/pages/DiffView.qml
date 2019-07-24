import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Pane {
    id: root
    padding: Const.mainPadding

    property variant controller: null

    StandardLabel {
        anchors.fill: parent
        text: Const.nothingToDisplayText
        visible: root.controller === null | list.count === 0
    }

    StandardListView {
        id: list
        anchors.fill: parent
        model: root.controller !== null ? root.controller.repository.diffModel : undefined

        delegate: Item {
            width: parent.width
            height: Const.elementHeight

            Rectangle {
                anchors.fill: parent
                color: if (operation === CDiffLine.Add) Const.fileStagedColor
                       else if (operation === CDiffLine.Delete) Const.fileModifiedColor
                       else Const.transparent
            }

            StandardText {
                anchors.fill: parent
                text: model.text
                elide: Text.ElideRight
            }
        }
    }
}
