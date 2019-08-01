import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import "../components"
import "../views"
import "../popups"

TitlePane {
    id: root

    property variant repository: null

    signal requestCopy(var commitId)
    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitBranchFrom(var commitId)
    signal requestCommitReset(var commitId)
    signal requestCommitRebase(var commitId)
    signal requestCommitSquash(var commitId)
    signal requestCommitMessageChange(var commitId, var commitMessage)

    title: Const.logText

    content: Item {
        anchors.fill: parent

        StandardLabel {
            anchors.fill: parent
            text: Const.listEmptyText
            visible: root.repository === null | logView.count === 0
        }

        LogView {
            id: logView
            anchors.fill: parent

            model: root.repository !== null ? root.repository.logModel : undefined

            onItemRightClicked: {
                menu.commitId = commitId
                menu.commitMessage = message
                menu.popup()
            }
        }

        LogMenu {
            id: menu
            repository: root.repository

            onRequestCopy: {
                root.requestCopy(commitId)
            }

            onRequestCommitDiffPrevious: {
                root.requestCommitDiffPrevious(commitId)
            }

            onRequestCommitBranchFrom: {
                root.requestCommitBranchFrom(commitId)
            }

            onRequestCommitReset: {
                root.requestCommitReset(commitId)
            }

            onRequestCommitRebase: {
                root.requestCommitRebase(commitId)
            }

            onRequestCommitSquash: {
                root.requestCommitSquash(commitId)
            }

            onRequestCommitMessageChange: {
                root.requestCommitMessageChange(commitId, commitMessage)
            }
        }
    }
}
