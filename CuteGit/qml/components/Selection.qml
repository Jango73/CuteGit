import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Rectangle {
    id: root
    width: targetWidth + Const.paneMargins
    height: targetHeight + Const.paneMargins * 0.5
    color: borderOnly ? Const.transparent : Material.primary
    radius: Const.mainRadius
    border.width: 2
    border.color: borderOnly ? Material.primary : Const.transparent
    opacity: 0
    state: show ? "on" : "off"

    property bool show: false
    property bool borderOnly: false
    property int targetWidth: 100
    property int targetHeight: 100

    states: [
        State {
            name: "off"
            PropertyChanges {
                target: root
                opacity: 0
            }
        },
        State {
            name: "on"
            PropertyChanges {
                target: root
                opacity: 1
            }
        }
    ]

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }
}
