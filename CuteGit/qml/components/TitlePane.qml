import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Pane {
    id: root
    anchors.margins: Const.paneMargins
    padding: Const.panePadding

    Material.elevation: Const.paneElevation

    property alias title: title.text
    property alias content: content.children

    StandardLabel {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Const.elementHeight
    }

    Item {
        id: content
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
