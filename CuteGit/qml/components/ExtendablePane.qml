import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

TitlePane {
    id: root

    //! This is the item to which the pane is parented when extended
    property Item extensionParent: null

    StandardButton {
        anchors.top: parent.top
        anchors.right: parent.right
        width: Const.microButtonWidth
        height: Const.microButtonHeight
        padding: 0
        flat: true
        text: root.state === "" ? "^" : "v"
        tipText: Const.tipExpandButton

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
                parent: root.extensionParent !== null ? root.extensionParent : parent
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
