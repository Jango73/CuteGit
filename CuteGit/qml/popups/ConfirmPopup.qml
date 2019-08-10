import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthNorm
    height: content.computedHeight + Const.mainPadding * 2

    property Action actionToTrigger: null

    property alias titleText: title.text
    property alias messageText: message.text

    contentItem: Item {
        id: content

        property int computedHeight: title.height + message.height + buttons.height

        Column {
            id: layout
            anchors.fill: parent

            StandardText {
                id: title
                width: parent.width
                height: Const.elementHeight * 2

                horizontalAlignment: Text.AlignHCenter
            }

            StandardText {
                id: message
                width: parent.width
                height: Const.elementHeight * 5
            }

            StandardToolBar {
                id: buttons

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
}
