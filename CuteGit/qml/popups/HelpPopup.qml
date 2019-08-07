import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"

StandardPopup {
    id: root

    property variant controller: null

    contentItem: Item {
        StandardLabel {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding

            horizontalAlignment: Text.AlignHCenter
            text: Const.helpTitleText
        }

        StandardText {
            id: copyrightText
            anchors.top: title.bottom
            anchors.left: parent.left
            width: parent.width
            height: parent.height * 0.15
        }

        StandardText {
            id: generalText
            anchors.top: copyrightText.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: parent.width * 0.5
        }

        StandardText {
            id: shortcutText
            anchors.top: copyrightText.bottom
            anchors.bottom: parent.bottom
            anchors.left: generalText.right
            width: parent.width * 0.5
        }

        Component.onCompleted: {
            copyrightText.text = Const.copyrightText.format(root.controller.version)
            generalText.text = Const.generalHelpText
            shortcutText.text = Const.shortcutHelpText
        }
    }
}
