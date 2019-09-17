import QtQuick 2.12
import QtQuick.Controls 2.5
import "../components"
import "../views"
import "../popups"

Item {
    id: root

    property variant repository: null

    signal requestMenu(var commitId, var message)
    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitDiffFrom(var commitId)
    signal requestCommitDiffTo(var commitId)

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

        model: root.repository ? root.repository.fileLogModel : undefined
        diffFromCommitId: root.repository ? root.repository.diffFromCommitId : ""
        diffToCommitId: root.repository ? root.repository.diffToCommitId : ""

        onItemRightClicked: root.requestMenu(commitId, message)
        onRequestCommitDiffPrevious: root.requestCommitDiffPrevious(commitId)
        onRequestCommitDiffFrom: root.requestCommitDiffFrom(commitId)
        onRequestCommitDiffTo: root.requestCommitDiffTo(commitId)
    }
}
