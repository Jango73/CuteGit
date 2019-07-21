import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: qsTr("&Repository")

    property variant controller: null
    property string commitId: ""
    property string commitMessage: ""

    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitRebase(var commitId)
    signal requestCommitMessageChange(var commitId, var commitMessage)
    signal requestCommitSquash(var commitId)

    Action {
        text: qsTr("&View diff with previous commit")
        enabled: false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffPrevious(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("&Rebase")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitRebase(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("&Change message...")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitMessageChange(root.commitId, root.commitMessage)
            }
        }
    }

    Action {
        text: qsTr("&Squash")
        enabled: false

        onTriggered: {
        }
    }
}
