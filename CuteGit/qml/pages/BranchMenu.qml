import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    property variant controller: null
    property bool canMerge: false

    signal requestSwitchToBranch()
    signal requestMergeBranch()
    signal requestDeleteBranch()

    Action {
        text: Const.switchToText

        onTriggered: {
            root.requestSwitchToBranch()
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
