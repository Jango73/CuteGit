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

        TabBar {
            id: tabBar
            anchors.top: copyrightText.bottom

            TabButton {
                width: implicitWidth
                text: Const.generalText
            }

            TabButton {
                width: implicitWidth
                text: Const.shortcutsText
            }
        }

        SwipeView {
            id: swipeView
            anchors.top: tabBar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            interactive: false
            clip: true
            currentIndex: tabBar.currentIndex

            StandardText {
                id: generalText
                padding: Const.mainPadding * 2
            }

            StandardText {
                id: shortcutText
                padding: Const.mainPadding * 2
            }
        }

        Component.onCompleted: {
            copyrightText.text = Const.copyrightText.format(root.controller.version)
            generalText.text = Const.generalHelpText
            shortcutText.text = Const.shortcutHelpText
        }
    }
}
