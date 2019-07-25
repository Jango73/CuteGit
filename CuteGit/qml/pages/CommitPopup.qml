import QtQuick 2.12
import QtQuick.Layouts 1.1
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
    property bool showFileList: true
    property bool amend: false

    property alias messageText: message.text

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
            text: root.showFileList ? root.amend ? qsTr("Amend") : qsTr("Commit") : qsTr("Change commit message")
        }

        StandardTextEdit {
            id: message
            anchors.top: title.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.3
            anchors.margins: Const.mainPadding
            focus: true
        }

        StandardText {
            id: fileList
            anchors.top: message.bottom
            anchors.bottom: buttons.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding
            text: qsTr("File list")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: root.showFileList
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

                enabled: message.text != "" || root.controller.repository.repositoryStatus !== CTreeFileModel.NoMerge

                onClicked: {
                    root.close()

                    if (root.showFileList) {
                        root.controller.repository.commit(message.text, root.amend)
                    } else {
                        root.controller.repository.changeCommitMessage(commitId, message.text)
                    }
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
