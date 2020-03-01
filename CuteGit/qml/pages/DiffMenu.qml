import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    /*! The repository for this view */
    property variant repository: null

    /*! The currently selected operation */
    property string operation: ""

    /*! The currently selected text */
    property string text: ""

    /*! Requests a copy action */
    signal requestCopyText(var text)

    Action {
        text: Const.diffCopyLineText

        onTriggered: {
            if (root.text !== "") {
                root.requestCopyText(root.text)
            }
        }
    }
}
