import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.5 as QC15
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.1 as QLP
import CuteGit 1.0
import "../components"
import "../views"
import "../popups"

Item {
    id: root

    property variant controller: null

    MainMenu {
        id: menu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        controller: root.controller
        repository: root.controller.currentRepository

        onRequestCloneRepository: cloneDialog.open()
        onRequestOpenRepository: openDialog.open()
        onRequestShortcuts: shortcuts.open()

        Component.onCompleted: {
            repositoryView = Qt.binding(function() { return container.getTab(container.currentIndex).item })
        }
    }

    StandardToolBar {
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
                action: menu.commitAction
                icon.source: Const.commitIcon
            }

            StandardToolButton {
                action: menu.amendAction
                icon.source: Const.commitIcon
            }
        }
    }

    QC15.TabView {
        id: container
        anchors.top: toolBar.bottom
        anchors.bottom: statusBar.top
        anchors.left: parent.left
        anchors.right: parent.right

        style: StandardTabViewStyle {
            canClose: true
            closeAction: tabCloseAction
        }

        Action {
            id: tabCloseAction

            onTriggered: {
                root.controller.removeRepository(source.index)
                container.removeTab(source.index)
            }
        }

        Repeater {
            model: root.controller.openRepositoryModel

            QC15.Tab {
                title: model.repository.repositoryName

                RepositoryView {
                    repository: model.repository
                    filesAsTree: menu.filesAsTree
                }
            }
        }

        onCountChanged: {
            currentIndex = count - 1
            root.controller.currentRepositoryIndex = currentIndex
        }

        onCurrentIndexChanged: {
            root.controller.currentRepositoryIndex = currentIndex
        }
    }

    Item {
        id: statusBar
        anchors.bottom: parent.bottom
        width: parent.width
        height: Const.elementHeight

        RowLayout {
            anchors.fill: parent
            StandardLabel {
                text: root.controller.statusText
            }
        }
    }

    ClonePopup {
        id: cloneDialog
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightSmall
        anchors.centerIn: parent

        controller: root.controller
    }

    QLP.FolderDialog {
        id: openDialog

        onAccepted: {
            root.controller.openRepository(folder)
        }
    }

    Popup {
        id: shortcuts
        width: root.width * Const.popupWidthNorm
        height: root.height * Const.popupHeightNorm
        anchors.centerIn: parent
        modal: true
        closePolicy: Popup.CloseOnEscape
        padding: Const.mainPadding

        StandardText {
            anchors.fill: parent
            text: Const.shortcutsText
        }
    }
}
