import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

TextEdit {
    id: root
    font.family: Theme.mainFontFamily
    font.pixelSize: Theme.mainFontSize
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignTop
    wrapMode: TextEdit.WordWrap
}
