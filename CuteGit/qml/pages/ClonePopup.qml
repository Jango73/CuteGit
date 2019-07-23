import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

Popup {
    id: root
    modal: true
    closePolicy: Popup.CloseOnEscape
    padding: Const.mainPadding

    Material.elevation: Const.popupElevation

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
            height: cancelButton.height * 1.1
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
