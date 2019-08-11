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
        height: Const.elementHeight

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

        StandardText {
            id: symbolText
            anchors.top: parent.top
            anchors.left: parent.left
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            text: model.graphSymbol
            font.family: "Courier"
            font.bold: true
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
            anchors.left: symbolText.right
            width: parent.width - symbolText.width
            height: parent.height

            Item {
                id: flickedDataZone
                width: parent.width
                height: parent.height
                clip: true

                // The row containing labels
                Row {
                    id: labelLayout
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: labelsRepeater.count > 0 ? Const.mainPadding : 0
                    spacing: Const.mainPadding
                    width: delegateItem.showLabels ? implicitWidth : 0

                    Behavior on width {
                        NumberAnimation {
                            duration: Const.componentFadingDuration
                            easing.type: Easing.InOutQuad
                        }
                    }

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
                    anchors.leftMargin: Const.mainPadding
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
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
        }
    }
}
