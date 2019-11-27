import QtQuick 2.12
import QtQuick.Controls 2.5
import "../components"
import "../views"
import "../popups"

Item {
    id: root

    property variant repository: null

    signal requestTextFilter(var text)
    signal requestMenu(var operation, var text)

    StandardLabel {
        anchors.fill: diffView
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Const.listEmptyText
        visible: root.repository === null | diffView.count === 0
    }

    StandardLabel {
        id: diffSource
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: root.repository ? root.repository.fileDiffModel.relativeSourceName : ""
    }

    StandardTextFilter {
        id: filter
        anchors.top: diffSource.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: Const.filterText
        tipText: Const.tipLineFilter

        onFilterTextChanged: root.requestTextFilter(text)
    }

    DiffView {
        id: diffView
        anchors.top: filter.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: root.repository !== null ? root.repository.fileDiffModelProxy : undefined

        onItemRightClicked: root.requestMenu(operation, text)
    }
}
