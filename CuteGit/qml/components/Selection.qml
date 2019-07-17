import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Rectangle {
    width: targetWidth + Const.paneMargins
    height: targetHeight + Const.paneMargins * 0.5
    color: borderOnly ? Const.transparent : Material.primary
    radius: Const.mainRadius
    border.width: 2
    border.color: borderOnly ? Material.primary : Const.transparent

    property bool borderOnly: false
    property int targetWidth: 100
    property int targetHeight: 100
}
