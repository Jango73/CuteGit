import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
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
    property string commitId: ""

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
            text: Const.branchFromText
        }

        StandardTextEdit {
            id: name
            anchors.top: title.bottom
            anchors.bottom: buttons.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding
            placeHolderText: Const.enterBranchNameHereText
            focus: true
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
                    root.close()
                    root.controller.repository.commitBranchFrom(root.commitId, name.text)
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
