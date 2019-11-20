import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"

StandardListView {
    id: root

    /*! If true, a graph symbol from the model is shown before the text */
    property bool hasSymbol: false

    /*! The currently selected commit */
    property string currentCommitId: ""

    /*! The id of the diff 'from' commit */
    property string diffFromCommitId: ""

    /*! The id of the diff 'to' commit */
    property string diffToCommitId: ""

    signal itemRightClicked(var commitId, var message)
    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitDiffFrom(var commitId)
    signal requestCommitDiffTo(var commitId)

    function isValidDate(d) {
        return d instanceof Date && !isNaN(d);
    }

    delegate: StandardListViewItem {
        id: delegateItem
        objectName: "root.delegate." + index
        width: parent.width
        expanded: true
        dataLoading: !model.messageIsComplete
        listView: root
        symbolText: root.hasSymbol ? model.graphSymbol : ""
        primaryText: fullText
        secondaryText: model.author + " - " + (isValidDate(model.date) ? model.date : "")
        selectionShown: index === root.currentIndex
        focusShown: root.activeFocus && index === root.currentIndex

        property string commitId: model.commitId
        property variant labels: model.labels
        property string fullText: (
                                      model.markedAsDiffFrom
                                      ? Const.logLabelFrom
                                      : model.markedAsDiffTo
                                        ? Const.logLabelTo
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
                        text: model.name
                        type: model.type
                    }
                }
            }
        ]

        onClicked: {
            if (mouse.button === Qt.RightButton) {
                root.itemRightClicked(model.commitId, model.fullMessage)
            }
        }

        onDoubleClicked: root.requestCommitDiffPrevious(root.currentCommitId)
    }

    onCountChanged: handleItemChange()
    onCurrentIndexChanged: handleItemChange()

    onSpacePressed: root.requestCommitDiffPrevious(root.currentCommitId)

    onEnterPressed: {
        if (root.diffFromCommitId === "") {
            root.requestCommitDiffFrom(root.currentCommitId)
        } else {
            root.requestCommitDiffTo(root.currentCommitId)
        }
    }

    function handleItemChange() {
        if (root.currentItem)
            root.currentCommitId = root.currentItem.commitId
        else
            root.currentCommitId = ""
    }
}
