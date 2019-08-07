import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.1 as QLP
import "js/Utils.js" as Utils
import "components"
import "pages"
import "views"
import "popups"

ApplicationWindow {
    id: root
    width: 1024
    height: 768
    visible: true

    property var ctrl: controller
    property var materialTheme: Material.theme

    Material.theme: Material.Dark
    Material.primary: Material.Teal
    Material.accent: Material.Green

    menuBar: MainMenu {
        id: menu

        controller: root.ctrl
        repository: root.ctrl.currentRepository
        materialTheme: root.materialTheme

        onRequestCloneRepository: cloneDialog.open()
        onRequestOpenRepository: openDialog.open()
        onRequestDarkTheme: root.setTheme(Material.Dark)
        onRequestLightTheme: root.setTheme(Material.Light)
        onRequestHelp: helpDialog.open()

        Component.onCompleted: {
            repositoryView = Qt.binding(function() {
                return mainSwipeView.count > 0 ? mainSwipeView.currentItem : null
            })
        }
    }

    header: StandardToolBar {
        id: toolBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: menu.bottom

        Row {
            spacing: Const.mainPadding

            StandardToolButton {
                action: menu.cloneRepositoryAction
                icon.source: Const.cloneIcon
            }

            StandardToolButton {
                action: menu.openRepositoryAction
                icon.source: Const.openIcon
            }

            ToolSeparator {}

            StandardToolButton {
                action: menu.fetchAction
                icon.source: Const.fetchIcon
            }

            StandardToolButton { action: menu.pullAction }
            StandardToolButton { action: menu.pushAction }

            ToolSeparator {}

            StandardToolButton {
                action: menu.stageSelectionAction
                text: Const.stageText
                icon.source: Const.stageIcon
            }

            StandardToolButton {
                action: menu.revertSelectionAction
                text: Const.revertText
                icon.source: Const.revertIcon
            }

            ToolSeparator {}

            StandardToolButton {
                action: menu.commitAction
                icon.source: Const.commitIcon
            }

            StandardToolButton {
                action: menu.amendAction
                icon.source: Const.commitIcon
            }

            ToolSeparator {}

            StandardToolButton {
                action: menu.saveStashAction
                icon.source: Const.saveStashIcon
            }

            StandardToolButton {
                action: menu.popStashAction
                icon.source: Const.popStashIcon
            }
        }
    }

    Item {
        id: clientZone
        anchors.fill: parent

        TabBar {
            id: mainTabBar
            anchors.top: parent.top

            Repeater {
                model: root.ctrl.openRepositoryModel

                TabButton {
                    width: implicitWidth
                    text: model.repository.repositoryName + " - " + model.repository.repositoryTypeString
                }
            }

            onCountChanged: {
                var indexToSet = root.ctrl.currentRepositoryIndexToSet()
                if (indexToSet !== -1)
                    currentIndex = indexToSet

                root.ctrl.currentRepositoryIndex = currentIndex
            }

            onCurrentIndexChanged: {
                root.ctrl.currentRepositoryIndex = currentIndex
            }
        }

        SwipeView {
            id: mainSwipeView
            anchors.top: mainTabBar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            interactive: false
            currentIndex: mainTabBar.currentIndex

            Repeater {
                model: root.ctrl.openRepositoryModel

                RepositoryView {
                    repository: model.repository
                    filesAsTree: menu.filesAsTree
                }
            }
        }

        StandardLabel {
            anchors.fill: parent
            visible: root.ctrl.openRepositoryModel.count === 0
            text: Const.emptyRepositoryTabText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    footer: Item {
        id: statusBar
        height: Const.elementHeight * 1.5

        RowLayout {
            anchors.fill: parent

            StandardLabel {
                text: root.ctrl.statusText
            }
        }
    }

    ClonePopup {
        id: cloneDialog
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightSmall
        anchors.centerIn: parent

        controller: root.ctrl

        onCloneBegins: statusTextHistory.open()
    }

    QLP.FolderDialog {
        id: openDialog

        onAccepted: {
            root.ctrl.openRepository(folder)
        }
    }

    HelpPopup {
        id: helpDialog
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent

        controller: root.ctrl
    }

    StandardPopup {
        id: statusTextHistory
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent

        StandardStringListView {
            anchors.fill: parent
            model: root.ctrl.statusTextHistory
        }
    }

    function setTheme(theme) {
        Material.theme = theme
    }
}
