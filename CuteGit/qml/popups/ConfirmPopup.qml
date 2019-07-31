import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

StandardPopup {
    id: root

    property Action actionToTrigger: null

    property alias titleText: title.text
    property alias messageText: message.text

    contentItem: Item {
        anchors.fill: parent

        StandardText {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding
            horizontalAlignment: Text.AlignHCenter
        }

        StandardText {
            id: message
            anchors.top: title.bottom
            anchors.bottom: buttons.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding
        }

        StandardToolBar {
            id: buttons
            width: parent.width
            height: cancelButton.height + Const.mainPadding
            anchors.bottom: parent.bottom

            Row {
                spacing: Const.mainPadding

                StandardToolButton {
                    action: Action {
                        id: okButton
                        text: Const.okText
                        onTriggered: {
                            if (root.actionToTrigger !== null) {
                                root.close()
                                root.actionToTrigger.trigger()
                            }
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
