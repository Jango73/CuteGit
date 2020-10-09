import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    property bool canRebaseOn: false    // Set by owner of this component
    property bool canMerge: false       // Set by owner of this component

    signal requestSwitchToBranch()
    signal requestRebaseOnBranch()
    signal requestMergeBranch()
    signal requestDeleteBranch()

    Action {
        text: Const.switchToText

        onTriggered: {
            root.requestSwitchToBranch()
        }
    }

    Action {
        enabled: root.canRebaseOn
        text: Const.rebaseOnText

        onTriggered: {
            root.requestRebaseOnBranch()
        }
    }

    Action {
        enabled: root.canMerge
        text: Const.mergeText

        onTriggered: {
            root.requestMergeBranch()
        }
    }

    Action {
        text: Const.deleteText

        onTriggered: {
            root.requestDeleteBranch()
        }
    }
}
