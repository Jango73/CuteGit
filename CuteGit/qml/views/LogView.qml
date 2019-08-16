import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../generalUtils.js" as Utils
import "../components"

StandardListView {
    id: root

    property bool hasSymbol: false

    signal itemRightClicked(var commitId, var message)

    delegate: StandardListViewItem {
        id: delegateItem
        width: parent.width
        expanded: true
        dataLoading: !model.messageIsComplete
        listView: parent
        symbolText: root.hasSymbol ? model.graphSymbol : ""
        primaryText: fullText
        secondaryText: model.author + " - " + model.date
        selectionShown: index === root.currentIndex
        focusShown: root.activeFocus && index === root.currentIndex

        property variant labels: model.labels
        property string fullText: (
                                      model.markedAsDiffFrom
                                      ? "[F] "
                                      : model.markedAsDiffTo
                                        ? "[T] "
                                        : ""
                                      ) + model.message

        secondaryZone: [
            // The row containing labels
            Row {
                id: labelLayout
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: Const.mainPadding
                spacing: Const.mainPadding
                width: implicitWidth

                Repeater {
                    id: labelsRepeater

                    model: delegateItem.labels

                    LogLabel {
                        x: Const.mainPadding
                        height: labelLayout.height
                        text: modelData
                    }
                }
            }
        ]

        onClicked: {
            root.currentIndex = index
            root.forceActiveFocus()

            if (mouse.button === Qt.RightButton) {
                root.itemRightClicked(model.commitId, model.fullMessage)
            }
        }
    }
}
