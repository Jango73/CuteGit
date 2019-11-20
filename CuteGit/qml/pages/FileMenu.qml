import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    property variant fileNames: null

    signal requestDelete(var fileNames)
    signal requestBlame(var fileNames)

    Action {
        text: Const.fileDeleteMenuText

        onTriggered: {
            if (root.name !== "") {
                root.requestDelete(root.fileNames)
            }
        }
    }

    Action {
        text: Const.fileBlameMenuText

        onTriggered: {
            if (root.name !== "") {
                root.requestBlame(root.fileNames)
            }
        }
    }
}
