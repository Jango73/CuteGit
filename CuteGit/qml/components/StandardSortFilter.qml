import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

Item {
    id: root
    height: filter.height

    property ListModel fieldModel: null
    property bool sortDirection: false

    signal requestFilter(var text)
    signal requestSortField(var field)
    signal requestSortDirection(var direction)

    StandardTextFilter {
        id: filter
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width * 0.5
        text: Const.filterText

        onFilterTextChanged: root.requestFilter(text)
    }

    StandardLabel {
        id: sortLabel
        anchors.top: parent.top
        anchors.left: filter.right
        height: filter.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Const.sortText
    }

    ComboBox {
        anchors.top: parent.top
        anchors.left: sortLabel.right
        anchors.right: sortDirection.left
        anchors.leftMargin: Const.mainPadding
        anchors.rightMargin: Const.mainPadding
        height: filter.height
        model: root.fieldModel
        textRole: "text"

        onCurrentIndexChanged: root.requestSortField(model.get(currentIndex).field)
    }

    StandardButton {
        id: sortDirection
        width: height
        height: filter.height
        anchors.right: parent.right
        anchors.rightMargin: Const.mainPadding
        text: root.sortDirection === false ? "v" : "^"
        onClicked: {
            root.sortDirection = !root.sortDirection
            root.requestSortDirection(root.sortDirection)
        }
    }
}
