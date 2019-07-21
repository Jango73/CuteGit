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

    signal requestCommitMessageChange(var commitId, var commitMessage)
    signal requestCommitSquash(var commitId)

    Action {
        text: qsTr("&Change commit message...")

        onTriggered: {
            if (root.commitId !== "") {
                root.requestCommitMessageChange(root.commitId, root.commitMessage)
            }
        }
    }

    Action {
        text: qsTr("&Squash...")

        onTriggered: {
        }
    }
}
