import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    /*! The repository for this view */
    property variant repository: null

    /*! The currently selected commit */
    property string commitId: ""

    /*! The currently selected commit's message */
    property string commitMessage: ""

    signal requestCopy(var commitId)
    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitTagOn(var commitId)
    signal requestCommitBranchFrom(var commitId)
    signal requestCommitReset(var commitId)
    signal requestCommitRebase(var commitId)
    signal requestCommitSquash(var commitId)
    signal requestCommitMessageChange(var commitId, var commitMessage)
    signal requestCommitDiffFrom(var commitId)
    signal requestCommitDiffTo(var commitId)

    Action {
        text: qsTr("&Copy commit Id")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCopy(root.commitId)
            }
        }
    }

    MenuSeparator { }

    Action {
        text: qsTr("Create ta&g here")
        enabled: root.repository ? root.repository.can(CEnums.TagOnCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitTagOn(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("&Branch from here")
        enabled: root.repository ? root.repository.can(CEnums.BranchFromCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitBranchFrom(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("Reset &to here")
        enabled: root.repository ? root.repository.can(CEnums.ResetToCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitReset(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("&Rebase here")
        enabled: root.repository ? root.repository.can(CEnums.RebaseOnCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitRebase(root.commitId)
            }
        }
    }

    MenuSeparator { }

    Action {
        text: qsTr("&Squash")
        enabled: root.repository ? root.repository.can(CEnums.SquashCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitSquash(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("Change &message")
        enabled: root.repository ? root.repository.can(CEnums.ChangeCommitMessage) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitMessageChange(root.commitId, root.commitMessage)
            }
        }
    }

    MenuSeparator { }

    Action {
        text: qsTr("&View diff with previous commit")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffPrevious(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("Mark as diff from")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffFrom(root.commitId)
            }
        }
    }

    Action {
        text: qsTr("Mark as diff to")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffTo(root.commitId)
            }
        }
    }
}
