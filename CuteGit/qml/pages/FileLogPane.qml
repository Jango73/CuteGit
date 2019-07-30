import QtQuick 2.12
import QtQuick.Controls 2.5
import "../components"
import "../views"
import "../popups"

Pane {
    id: root
    padding: Const.mainPadding

    property variant repository: null

    StandardLabel {
        anchors.fill: parent
        text: Const.nothingToDisplayText
        visible: root.repository === null | logView.count === 0
    }

    LogView {
        id: logView
        anchors.fill: parent

        model: root.repository !== null ? root.repository.fileLogModel : undefined

        onItemRightClicked: {
            menu.commitId = commitId
            menu.commitMessage = message
            menu.popup()
        }
    }
}
