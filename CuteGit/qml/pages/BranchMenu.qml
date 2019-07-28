import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    property variant controller: null

    signal requestDeleteBranch()

    Action {
        text: qsTr("&Delete")

        onTriggered: {
            root.requestDeleteBranch()
        }
    }
}
