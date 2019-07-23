import QtQuick 2.12
import QtQuick.Controls 2.5
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
        model: root.controller !== null ? root.controller.treeFileModel.fileLogModel : undefined

        delegate: ElideText {
            width: parent.width
            height: Const.elementHeight
            text: display
        }
    }
}
