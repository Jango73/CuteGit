import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    property variant repository: null
    property string commitId: ""
    property string commitMessage: ""

    signal requestCopy(var commitId)
    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitBranchFrom(var commitId)
    signal requestCommitReset(var commitId)
    signal requestCommitRebase(var commitId)
    signal requestCommitSquash(var commitId)
    signal requestCommitMessageChange(var commitId, var commitMessage)

    Action {
        text: qsTr("&Copy commit Id")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCopy(root.commitId)
            }
        }
    }

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
        text: qsTr("&Branch from")
        enabled: root.repository.can(CEnums.BranchFromCommit)

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitBranchFrom(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("Reset &to")
        enabled: root.repository.can(CEnums.ResetToCommit)

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitReset(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("&Rebase here")
        enabled: root.repository.can(CEnums.RebaseOnCommit)

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitRebase(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("&Squash")
        enabled: root.repository.can(CEnums.SquashCommit)

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitSquash(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("Change &message")
        enabled: root.repository.can(CEnums.ChangeCommitMessage)

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitMessageChange(root.commitId, root.commitMessage)
            }
        }
    }
}
