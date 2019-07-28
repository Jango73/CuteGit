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

        property variant labels: model.labels

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
            anchors.centerIn: dataZone
            targetWidth: dataZone.width
            targetHeight: dataZone.height
            visible: index === root.currentIndex

            FocusIndicator {
                anchors.fill: parent
                visible: root.activeFocus
            }
        }

        Item {
            id: dataZone
            anchors.centerIn: parent
            width: parent.width - Const.smallPadding
            height: parent.height - Const.smallPadding

            Item {
                id: messageField
                width: parent.width * 0.55

                Row {
                    id: labels
                    width: addedWidth

                    property int addedWidth: 0

                    Repeater {
                        model: delegateItem.labels

                        Item {
                            width: labelText.width
                            height: parent.height

                            Selection {
                                anchors.centerIn: labelText
                                targetWidth: labelText.width
                                targetHeight: labelText.height
                                color: Material.accent
                            }

                            StandardText {
                                id: labelText
                                color: Material.background
                                text: modelData
                            }

                            Component.onCompleted: {
                                labels.addedWidth += width
                            }

                            Component.onDestruction: {
                                labels.addedWidth -= width
                            }
                        }
                    }
                }

                ElideText {
                    height: dataZone.height
                    anchors.left: labels.right
                    anchors.right: parent.right
                    anchors.leftMargin: Const.mainPadding
                    verticalAlignment: Text.AlignVCenter
                    color: selection.visible ? Material.background : Material.foreground
                    text: model.message
                }
            }

            ElideText {
                id: authorField
                anchors.left: messageField.right
                width: parent.width * 0.25
                height: dataZone.height
                verticalAlignment: Text.AlignVCenter
                color: selection.visible ? Material.background : Material.foreground
                text: model.author
            }

            ElideText {
                id: dateField
                anchors.left: authorField.right
                width: parent.width * 0.2
                height: dataZone.height
                verticalAlignment: Text.AlignVCenter
                color: selection.visible ? Material.background : Material.foreground
                text: model.date
            }
        }
    }
}
