import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"
import "../views"
import "../popups"

ExtendablePane {
    id: root

    property variant repository: null

    signal requestMenu(var commitId, var message)
    signal requestCommitDiffPrevious(var commitId)
    signal requestCommitDiffFrom(var commitId)
    signal requestCommitDiffTo(var commitId)

    content: [
        TabBar {
            id: tabBar
            anchors.top: parent.top

            TabButton {
                width: implicitWidth
                text: Const.diffText
            }

            TabButton {
                width: implicitWidth
                text: Const.logText
            }
        },

        SwipeView {
            id: swipeView
            anchors.top: tabBar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: Const.smallPadding
            interactive: false
            currentIndex: tabBar.currentIndex
            clip: true

            DiffView {
                id: diffView
                repository: root.repository
            }

            FileLogPane {
                id: fileLogPane
                repository: root.repository

                onRequestMenu: root.requestMenu(commitId, message)
                onRequestCommitDiffPrevious: root.requestCommitDiffPrevious(commitId)
                onRequestCommitDiffFrom: root.requestCommitDiffFrom(commitId)
                onRequestCommitDiffTo: root.requestCommitDiffTo(commitId)
            }
        }
    ]

    function activateFileDiffView() {
        tabBar.currentIndex = 0
        diffView.forceActiveFocus()
    }
}
