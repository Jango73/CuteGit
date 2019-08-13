import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthNorm
    title: Const.helpTitleText

    property variant controller: null

    controls: [
        StandardText {
            id: copyrightText
            width: parent.width
            height: parent.height * 0.15
        },

        TabBar {
            id: tabBar
            width: parent.width
            height: implicitHeight

            TabButton {
                width: implicitWidth
                text: Const.generalText
            }

            TabButton {
                width: implicitWidth
                text: Const.shortcutsText
            }
        },

        SwipeView {
            id: swipeView
            width: parent.width
            height: Const.elementHeight * 20
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
    ]

    Component.onCompleted: {
        copyrightText.text = Const.copyrightText.format(root.controller.version)
        generalText.text = Const.generalHelpText
        shortcutText.text = Const.shortcutHelpText
    }
}
