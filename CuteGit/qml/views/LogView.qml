import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../generalUtils.js" as Utils
import "../components"

StandardListView {
    id: root

    signal itemRightClicked(var commitId, var message)

    delegate: Item {
        id: delegateItem
        width: parent.width
        height: Const.listViewItemHeight

        property variant labels: model.labels
        property bool showLabels: true

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            hoverEnabled: true

            onEntered: {
                if (!Utils.itemInsideParent(messageFieldText))
                    delegateItem.showLabels = false
            }

            onExited: {
                delegateItem.showLabels = true
            }

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
            anchors.fill: dataZone
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
                id: flickedDataZone
                width: parent.width * 0.6
                height: parent.height
                clip: true

                // The row containing labels
                Row {
                    id: labelLayout
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: Const.mainPadding
                    spacing: Const.mainPadding
                    width: delegateItem.showLabels ? implicitWidth : 0

                    Repeater {
                        id: labelsRepeater

                        model: delegateItem.labels

                        LogLabel {
                            x: Const.mainPadding
                            height: labelLayout.height
                            text: modelData
                            visible: delegateItem.showLabels
                        }
                    }
                }

                TextOverSelection {
                    id: messageFieldText
                    anchors.top: parent.top
                    anchors.left: labelLayout.right
                    anchors.topMargin: Const.mainFontSize * 0.2
                    anchors.leftMargin: Const.mainPadding
                    height: parent.height
                    text: (
                              model.markedAsDiffFrom
                              ? "[F]  "
                              : model.markedAsDiffTo
                                ? "[T] "
                                : ""
                              ) + model.message

                    selection: selection
                }
            }

            TextOverSelection {
                id: authorField
                anchors.left: flickedDataZone.right
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
