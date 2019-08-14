import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "../components"
import ".."

StandardPane {
    id: root

    property variant repository: null
    property string branchName: ""

    signal requestMergeBranch(var name)
    signal requestDeleteBranch(var name)

    TabBar {
        id: tabBar
        anchors.top: parent.top

        TabButton {
            width: implicitWidth
            text: Const.branchesText
        }

        TabButton {
            width: implicitWidth
            text: Const.tagsText
        }
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: Const.smallPadding
        interactive: false
        clip: true
        currentIndex: tabBar.currentIndex

        StandardListView {
            id: branchList
            visible: count > 0

            model: root.repository ? root.repository.branchModel : undefined

            delegate: StandardListViewItem {
                width: parent.width
                listView: branchList
                selectionShown: model.name === root.repository.currentBranch
                primaryText: model.name

                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        root.branchName = model.name
                        branchMenu.canMerge = (model.name !== root.repository.currentBranch)
                        branchMenu.popup()
                    }
                }

                onDoubleClicked: {
                    root.repository.currentBranch = model.name
                }
            }

            BranchMenu {
                id: branchMenu

                onRequestSwitchToBranch: {
                    root.repository.currentBranch = root.branchName
                }

                onRequestMergeBranch: {
                    root.requestMergeBranch(root.branchName)
                }

                onRequestDeleteBranch: {
                    root.requestDeleteBranch(root.branchName)
                }
            }
        }

        StandardListView {
            id: tagList
            visible: count > 0

            model: root.repository ? root.repository.tagModel : undefined

            delegate: StandardListViewItem {
                width: parent.width
                listView: tagList
                selectionShown: false
                primaryText: model.name
            }
        }
    }
}
