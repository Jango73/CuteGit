import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import CuteGit 1.0
import "../components"

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthNorm

    property Action actionToTrigger: null

    property alias messageText: message.text

    controls: [
        StandardText {
            id: message
            width: parent.width
            height: Const.elementHeight * 5
        }
    ]

    buttons: [
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
