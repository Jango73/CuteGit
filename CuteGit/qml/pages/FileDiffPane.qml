import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"
import "../views"
import "../popups"

Item {
    id: root

    property variant repository: null

    /*! Requests to show the menu */
    signal requestMenu(var operation, var text)

    /*! Requests a filter change */
    signal requestTextFilter(var text)

    /*! Requests to save the diff as a patch */
    signal requestSaveAsPatch()

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

    RowLayout {
        id: filterLayout
        anchors.top: diffSource.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: Const.mainPadding

        StandardTextFilter {
            id: filter
            Layout.fillWidth: true
            text: Const.filterText
            tipText: Const.tipLineFilter

            onFilterTextChanged: root.requestTextFilter(text)
        }

        StandardButton {
            id: saveAsPatch
            text: Const.saveAsPatchText
            enabled: !diffView.empty

            onClicked: {
                root.requestSaveAsPatch()
            }
        }
    }

    DiffView {
        id: diffView
        anchors.top: filterLayout.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        model: root.repository !== null ? root.repository.fileDiffModelProxy : undefined

        onItemRightClicked: root.requestMenu(operation, text)
    }
}
