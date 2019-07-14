import QtQuick 2.12
import QtQuick.Controls 2.5
import "../components"

Pane {
    id: root
    padding: 0

    property variant controller: null

    StandardListView {
        anchors.fill: parent
        model: root.controller !== null ? root.controller.diffModel : undefined

        delegate: StandardText {
            width: parent.width
            height: Const.elementHeight
            text: display
        }
    }
}
