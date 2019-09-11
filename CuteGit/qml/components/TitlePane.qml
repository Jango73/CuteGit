import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

StandardPane {
    id: root

    property bool showTitle: true

    property alias title: title.text
    property alias content: content.children

    StandardLabel {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: visible ? Const.elementHeight : 0
        visible: showTitle && text !== ""
    }

    Item {
        id: content
        anchors.top: title.visible ? title.bottom : parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
