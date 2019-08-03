import QtQuick 2.12
import QtQuick.Layouts 1.3
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
        height: typeof model.labels !== "undefined" && model.labels !== null
                ? Const.elementHeight * (model.labels.length + 1)
                : Const.elementHeight

        property variant labels: model.labels

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            onClicked: {
                root.currentIndex = index
                root.forceActiveFocus()

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
            show: index === root.currentIndex

            FocusIndicator {
                anchors.fill: parent
                visible: root.activeFocus
            }
        }

        Item {
            id: dataZone
            anchors.centerIn: parent
            width: parent.width - Const.smallPadding
            height: parent.height

            Item {
                id: messageField
                width: parent.width * 0.6
                height: parent.height

                TextOverSelection {
                    id: messageFieldText
                    anchors.top: parent.top
                    width: parent.width
                    height: Const.elementHeight
                    verticalAlignment: Text.AlignVCenter
                    text: model.message

                    selection: selection
                }

                // The column containing labels
                Column {
                    id: labelLayout
                    anchors.top: messageFieldText.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 0

                    Repeater {
                        id: labelsRepeater

                        model: delegateItem.labels

                        LogLabel {
                            x: Const.mainPadding
                            height: Const.elementHeight
                            text: modelData
                        }
                    }
                }
            }

            TextOverSelection {
                id: authorField
                anchors.left: messageField.right
                width: parent.width * 0.2
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                text: model.author

                selection: selection
            }

            TextOverSelection {
                id: dateField
                anchors.left: authorField.right
                width: parent.width * 0.2
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                text: model.date

                selection: selection
            }
        }
    }
}
