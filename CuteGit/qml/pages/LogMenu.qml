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

    signal requestCopyText(var text)
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
        text: Const.logCopyCommitIdMenuText

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCopyText(root.commitId)
            }
        }
    }

    Action {
        text: Const.logCopyCommitMessageMenuText

        onTriggered: {
            if (root.commitMessage !== "") {
                root.requestCopyText(root.commitMessage)
            }
        }
    }

    MenuSeparator {}

    Action {
        text: Const.logCreateTagMenuText
        enabled: root.repository ? root.repository.can(CEnums.TagOnCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitTagOn(root.commitId)
            }
        }
    }

    Action {
        text: Const.logCreateBranchMenuText
        enabled: root.repository ? root.repository.can(CEnums.BranchFromCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitBranchFrom(root.commitId)
            }
        }
    }

    Action {
        text: Const.logResetMenuText
        enabled: root.repository ? root.repository.can(CEnums.ResetToCommit) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitReset(root.commitId)
            }
        }
    }

    MenuSeparator {}

    Action {
        text: Const.logRebaseMenuText
        enabled: root.repository ? (root.repository.can(CEnums.RebaseOnCommit) && !root.repository.hasModifiedFiles) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitRebase(root.commitId)
            }
        }
    }

    Action {
        text: Const.logSquashMenuText
        enabled: root.repository ? (root.repository.can(CEnums.SquashCommit) && !root.repository.hasModifiedFiles) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitSquash(root.commitId)
            }
        }
    }

    Action {
        text: Const.logChangeMessageMenuText
        enabled: root.repository ? (root.repository.can(CEnums.ChangeCommitMessage) && !root.repository.hasModifiedFiles) : false

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitMessageChange(root.commitId, root.commitMessage)
            }
        }
    }

    MenuSeparator {}

    Action {
        text: Const.logDiffPreviousCommitMenuText

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffPrevious(root.commitId)
            }
        }
    }

    Action {
        text: Const.logMarkDiffFrom

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffFrom(root.commitId)
            }
        }
    }

    Action {
        text: Const.logMarkDiffTo

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitDiffTo(root.commitId)
            }
        }
    }
}
