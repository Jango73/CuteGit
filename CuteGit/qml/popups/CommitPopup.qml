import QtQuick 2.12
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"
import "../views"

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthNorm
    title: showFileList ? amend ? Const.amendText : Const.commitText : Const.changeCommitMessageText

    property variant repository: null
    property string commitId: ""
    property bool showFileList: true
    property bool amend: false

    property alias messageText: message.text
    property alias messageEnabled: message.enabled

    controls: [
        StandardTextEdit {
            id: message
            width: parent.width
            height: Const.elementHeight * 10

            placeHolderText: Const.enterMessageHereText
            focus: true
        },

        TitlePane {
            id: filePane
            width: parent.width
            height: Const.listViewItemHeight * 10

            title: Const.stagedFilesForCommitText

            content: StandardListView {
                id: fileList
                anchors.fill: parent
                visible: root.showFileList

                model: root.repository
                       ? root.repository.stagedFileModelProxy
                       : undefined

                delegate: Item {
                    id: dlg
                    width: parent.width
                    height: Const.listViewItemHeight

                    Item {
                        id: listViewStatus
                        width: Const.elementHeight
                        height: parent.height
                        anchors.left: parent.left

                        ElideText {
                            id: listViewStatusText
                            width: parent.width - Const.smallPadding
                            anchors.centerIn: parent
                            text: model.status
                        }
                    }

                    Item {
                        id: listViewFileName
                        anchors.left: listViewStatus.right
                        width: parent.width * 0.4
                        height: parent.height

                        StandardText {
                            id: listViewFileNameText
                            width: parent.width - Const.smallPadding
                            anchors.centerIn: parent
                            text: model.fileName
                        }
                    }
                }
            }
        }
    ]

    buttons: [
        StandardToolButton {
            action: Action {
                id: okButton
                text: Const.okText
                enabled: root.repository
                         ? message.text != "" || root.repository.repositoryStatus !== CEnums.NoMerge
                         : false
                onTriggered: {
                    root.close()

                    if (root.showFileList) {
                        root.repository.commit(message.text, root.amend)
                    } else {
                        root.repository.changeCommitMessage(commitId, message.text)
                    }
                }
            }
        },

        StandardToolButton {
            action: Action {
                id: cancelButton
                text: Const.cancelText
                onTriggered: {
                    root.close()
                }
            }
        }
    ]
}
