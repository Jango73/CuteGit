import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root

    property alias text: text.text
    property alias horizontalAlignment: text.horizontalAlignment
    property alias verticalAlignment: text.verticalAlignment

    Rectangle {
        id: background
        anchors.fill: parent
        color: Const.transparent
        border.width: 1
        border.color: Material.foreground
    }

    TextEdit {
        id: text
        anchors.fill: parent
        anchors.margins: Const.mainPadding
        font.family: Const.mainFontFamily
        font.pixelSize: Const.mainFontSize
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        wrapMode: TextEdit.WordWrap
        color: Material.foreground
        selectByMouse: true
        focus: true
    }

    StandardText {
        id: placeholder
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Const.enterMessageHereText
        visible: text.text === ""
        color: Const.foregroundLighter
    }
}
