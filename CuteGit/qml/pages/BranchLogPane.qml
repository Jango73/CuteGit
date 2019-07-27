import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import "../components"

TitlePane {
    id: root

    property variant controller: null

    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitBranchFrom(var commitId)
    signal requestCommitReset(var commitId)
    signal requestCommitRebase(var commitId)
    signal requestCommitSquash(var commitId)
    signal requestCommitMessageChange(var commitId, var commitMessage)

    title: Const.logText

    content: Item {
        anchors.fill: parent

        LogView {
            id: logView
            anchors.fill: parent

            model: root.controller !== null ? root.controller.repository.logModel : undefined

            onItemRightClicked: {
                menu.commitId = commitId
                menu.commitMessage = message
                menu.popup()
            }
        }

        LogMenu {
            id: menu
            controller: root.controller

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
