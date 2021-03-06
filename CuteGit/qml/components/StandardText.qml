import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Text {
    property bool asLabel: false

    font.family: Const.mainFontFamily
    font.pixelSize: Const.mainFontSize
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignTop
    wrapMode: Text.WordWrap
    color: asLabel
           ? Material.backgroundDimColor
           : Material.foreground
}
