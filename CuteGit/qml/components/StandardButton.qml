import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

Button {
    property string tipText: ""
    implicitHeight: Const.buttonHeight
    ToolTip.delay: Const.toolTipDelay
    ToolTip.timeout: Const.toolTipTimeout
    ToolTip.visible: hovered
    ToolTip.text: tipText
}
