import QtQuick 2.12
import QtQuick.Controls 2.5
import "../components"
import "../views"
import "../popups"

Item {
    id: root

    property variant repository: null

    signal requestMenu(var commitId, var message)

    StandardLabel {
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Const.listEmptyText
        visible: root.repository === null | logView.count === 0
    }

    LogView {
        id: logView
        anchors.fill: parent

        model: root.repository !== null ? root.repository.fileLogModel : undefined

        onItemRightClicked: {
            root.requestMenu(commitId, message)
        }
    }
}
