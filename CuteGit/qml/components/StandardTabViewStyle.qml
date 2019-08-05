import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

TabViewStyle {
    id: root
    frameOverlap: 1

    property Action closeAction: null
    property bool canClose: false
    property int minSize: 80

    tab: Item {
        id: tab
        implicitWidth: root.canClose
                       ? Math.max(text.width + Const.mainPadding * 2 + Const.elementHeight, root.minSize)
                       : Math.max(text.width + Const.mainPadding, root.minSize)
        implicitHeight: Const.elementHeight + Const.mainPadding * 0.5
        clip: true

        Rectangle {
            width: parent.width
            height: parent.height + Const.mainRadius
            color: styleData.selected ? Material.primary : Material.background
            radius: Const.mainRadius
            border.width: 1
            border.color: Material.primary
        }

        FocusIndicator {
            anchors.fill: parent
            visible: styleData.activeFocus
        }

        StandardText {
            id: text
            anchors.left: root.canClose ? parent.left : undefined
            anchors.leftMargin: root.canClose ? Const.mainPadding : 0
            anchors.horizontalCenter: root.canClose ? undefined : parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: styleData.title
            color: styleData.selected ? Material.background : Material.foreground
        }

        Item {
            anchors.right: parent.right
            width: parent.height
            height: parent.height
            visible: root.canClose && styleData.selected

            MouseArea {
                anchors.fill: parent

                property var index: styleData.index

                onClicked: {
                    if (root.closeAction !== null)
                        root.closeAction.trigger(this)
                }
            }

            Item {
                id: cross
                anchors.centerIn: parent
                width: parent.width * 0.6
                height: parent.height * 0.6

                Rectangle {
                    x: 0
                    y: parent.height * 0.5 - height * 0.3
                    width: parent.width
                    height: 3
                    color: "black"
                }

                Rectangle {
                    x: parent.width * 0.5 - width * 0.3
                    y: 0
                    width: 3
                    height: parent.height
                    color: "black"
                }

                transform: Rotation {
                    origin.x: cross.width * 0.5
                    origin.y: cross.height * 0.5
                    angle: 45
                }
            }
        }
    }

    frame: Rectangle {
        color: Material.background
    }
}
