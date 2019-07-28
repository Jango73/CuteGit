import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"

StandardPopup {
    id: root

    property variant controller: null

    Component.onCompleted: {
        root.forceActiveFocus()
    }

    contentItem: Item {
        anchors.fill: parent

        StandardText {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding

            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Clone repository")
        }

        RowLayout {
            id: buttons
            width: parent.width
            height: cancelButton.height + Const.mainPadding
            anchors.bottom: parent.bottom

            StandardButton {
                id: okButton
                Layout.alignment: Qt.AlignCenter
                text: Const.okText

                onClicked: {
                }
            }

            StandardButton {
                id: cancelButton
                Layout.alignment: Qt.AlignCenter
                text: Const.cancelText

                onClicked: {
                    root.close()
                }
            }
        }
    }
}
