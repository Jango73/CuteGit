import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Item {
    id: root
    anchors.margins: Const.paneMargins
    implicitHeight: title.height + theText.height

    property alias title: title.text
    property alias text: theText.text

    StandardLabel {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    StandardTextField {
        id: theText
        anchors.top: title.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
