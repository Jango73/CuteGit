import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

TextField {
    property string tipText: ""
    font.family: Const.mainFontFamily
    font.pixelSize: Const.mainFontSize
    selectByMouse: true
    ToolTip.delay: Const.toolTipDelay
    ToolTip.timeout: Const.toolTipTimeout
    ToolTip.visible: hovered
    ToolTip.text: tipText
}
