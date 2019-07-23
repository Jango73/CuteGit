import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"

StandardListView {
    id: root
    activeFocusOnTab: true

    signal itemRightClicked(var commitId, var message)

    delegate: Item {
        id: delegateItem
        width: parent.width
        height: Const.elementHeight + Const.smallPadding

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            onClicked: {
                root.currentIndex = index

                if (mouse.button === Qt.RightButton) {
                    root.itemRightClicked(model.commitId, model.message)
                }
            }
        }

        Selection {
            id: selection
            targetWidth: data.width
            targetHeight: data.height
            anchors.centerIn: data
            visible: index === root.currentIndex

            FocusIndicator {
                anchors.fill: parent
                visible: root.activeFocus
            }
        }

        Item {
            id: data
            anchors.centerIn: parent
            width: parent.width - Const.smallPadding
            height: parent.height - Const.smallPadding

            ElideText {
                id: messageField
                width: parent.width * 0.55
                verticalAlignment: Text.AlignVCenter
                color: selection.visible ? Material.background : Material.foreground
                text: model.message
            }

            ElideText {
                id: authorField
                anchors.left: messageField.right
                width: parent.width * 0.25
                verticalAlignment: Text.AlignVCenter
                color: selection.visible ? Material.background : Material.foreground
                text: model.author
            }

            ElideText {
                id: dateField
                anchors.left: authorField.right
                width: parent.width * 0.2
                verticalAlignment: Text.AlignVCenter
                color: selection.visible ? Material.background : Material.foreground
                text: model.date
            }
        }
    }
}
