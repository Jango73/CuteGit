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

            ToolButton { action: menu.cloneRepositoryAction }
            ToolButton { action: menu.openRepositoryAction }
            ToolSeparator {}
            ToolButton { action: menu.pullAction }
            ToolButton { action: menu.pushAction }
            ToolSeparator {}
            ToolButton { action: menu.commitAction }
            ToolButton { action: menu.amendAction }
        }
    }

    QC15.TabView {
        id: container
        anchors.top: toolBar.bottom
        anchors.bottom: parent.bottom
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
                // container.removeTab(source.index)
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
            root.controller.currentRepositoryIndex = currentIndex
        }

        onCurrentIndexChanged: {
            root.controller.currentRepositoryIndex = currentIndex
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
