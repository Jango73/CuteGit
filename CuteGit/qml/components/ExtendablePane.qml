import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

TitlePane {
    id: root

    property Item extensionParent: null

    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        width: Const.elementHeight * 1.2
        height: Const.elementHeight * 1.2
        padding: 0
        flat: true
        text: root.state === "" ? "^" : "v"

        onClicked: {
            if (root.extensionParent !== null)
                root.state = (root.state === "" ? "extended" : "")
        }
    }

    states: [
        State {
            name: ""
        },
        State {
            name: "extended"
            ParentChange {
                target: root
                parent: root.extensionParent
            }
            PropertyChanges {
                target: root
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                z: 999999
            }
        }
    ]
}
