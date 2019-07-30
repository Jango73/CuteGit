import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"
import "../views"

StandardPopup {
    id: root

    property variant repository: null
    property string commitId: ""
    property bool showFileList: true
    property bool amend: false

    property alias messageText: message.text
    property alias messageEnabled: message.enabled

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
            anchors.margins: Const.mainPadding
            height: parent.height * 0.3

            placeHolderText: Const.enterMessageHereText
            focus: true
        }

        TitlePane {
            id: filePane
            anchors.top: message.bottom
            anchors.bottom: buttons.top
            anchors.left: parent.left
            anchors.right: parent.right

            title: Const.filesText

            content: FlatFileView {
                id: fileList
                anchors.fill: parent
                anchors.margins: Const.mainPadding
                repository: root.repository
                visible: root.showFileList
                mouseActive: false
            }
        }

        StandardToolBar {
            id: buttons
            width: parent.width
            anchors.bottom: parent.bottom

            Row {
                spacing: Const.mainPadding

                ToolButton {
                    action: Action {
                        id: okButton
                        text: Const.okText
                        enabled: message.text != "" || root.repository.repositoryStatus !== CEnums.NoMerge
                        onTriggered: {
                            root.close()

                            if (root.showFileList) {
                                root.repository.commit(message.text, root.amend)
                            } else {
                                root.repository.changeCommitMessage(commitId, message.text)
                            }
                        }
                    }
                }

                ToolButton {
                    action: Action {
                        id: cancelButton
                        text: Const.cancelText
                        onTriggered: {
                            root.close()
                        }
                    }
                }
            }
        }
    }
}
