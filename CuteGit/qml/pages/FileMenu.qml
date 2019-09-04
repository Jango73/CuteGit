import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

Menu {
    id: root
    title: ""

    property variant repository: null
    property string name: ""

    signal requestDelete(var name)

    Action {
        text: qsTr("&Delete")

        onTriggered: {
            if (root.name !== "") {
                root.requestDelete(root.name)
            }
        }
    }
}
