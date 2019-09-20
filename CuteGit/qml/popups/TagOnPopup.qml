import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"
import "../views"

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthNorm
    height: content.computedHeight + Const.mainPadding * 2

    property variant repository: null
    property string commitId: ""

    property alias messageText: message.text
    property alias messageEnabled: message.enabled

    contentItem: Item {
        id: content

        property int computedHeight: title.height + name.height + message.height + buttons.height

        Column {
            id: layout
            anchors.fill: parent

            StandardText {
                id: title
                width: parent.width
                height: Const.elementHeight * 2
                horizontalAlignment: Text.AlignHCenter
                text: Const.tagText
            }

            TitleTextField {
                id: name
                width: parent.width
                height: implicitHeight
                title: Const.nameText
                focus: true
            }

            TitleTextField {
                id: message
                width: parent.width
                height: implicitHeight
                title: Const.messageText
                focus: true
            }

            StandardToolBar {
                id: buttons

                Row {
                    spacing: Const.mainPadding

                    StandardToolButton {
                        action: Action {
                            id: okButton
                            text: Const.okText
                            enabled: message.text != ""
                            onTriggered: {
                                root.close()

                                root.repository.createTagOnCommit(root.commitId, name.text, message.text)
                            }
                        }
                    }

                    StandardToolButton {
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
}
