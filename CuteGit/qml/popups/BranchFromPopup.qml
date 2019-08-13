import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthSmall
    title: Const.branchFromText

    property variant repository: null
    property string commitId: ""

    controls: [
        StandardTextEdit {
            id: name
            width: parent.width
            height: Const.elementHeight * 2

            placeHolderText: Const.enterBranchNameHereText
            focus: true
        }
    ]

    buttons: [
        StandardToolButton {
            action: Action {
                id: okButton
                text: Const.okText
                onTriggered: {
                    root.close()
                    root.repository.commitBranchFrom(root.commitId, name.text)
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
