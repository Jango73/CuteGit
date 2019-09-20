import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import Qt.labs.platform 1.1 as QLP
import CuteGit 1.0
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
    property var currentRepositoryView: null

    Material.primary: Material.Teal
    Material.accent: Material.Green

    Component.onCompleted: {
        currentRepositoryView = Qt.binding(function() {
            return mainSwipeView.count > 0 ? mainSwipeView.currentItem : null
        })

        Material.theme = Material.Dark  // This is to force material change (else font is white in light theme)
        setTheme(ctrl.theme === Const.themeMaterialDark ? Material.Dark : Material.Light)
    }

    //--------------------------------------------------------------------------------
    // Menu

    menuBar: MainMenu {
        id: menu

        repositoryView: root.currentRepositoryView
        controller: root.ctrl
        repository: root.ctrl.currentRepository
        materialTheme: root.materialTheme

        onRequestCloneRepository: cloneDialog.open()
        onRequestOpenRepository: openDialog.open()
        onRequestDarkTheme: root.setTheme(Material.Dark)
        onRequestLightTheme: root.setTheme(Material.Light)
        onRequestHelp: helpDialog.open()
    }

    //--------------------------------------------------------------------------------
    // Toolbar

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

            StandardToolButton {
                action: menu.pullAction
                icon.source: Const.pullIcon
            }
            StandardToolButton {
                action: menu.pushAction
                icon.source: Const.pushIcon
            }

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

    //--------------------------------------------------------------------------------
    // Work area

    Item {
        id: clientZone
        anchors.fill: parent

        TabBar {
            id: mainTabBar
            anchors.top: parent.top

            Action {
                id: tabCloseAction

                onTriggered: {
                    root.ctrl.removeRepository(source.tabIndex)
                }
            }

            Repeater {
                model: root.ctrl.openRepositoryModel

                ClosableTabButton {
                    width: implicitWidth
                    text: model.repository.repositoryName
                    closeAction: tabCloseAction
                    tabIndex: index
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
            anchors.topMargin: Const.smallPadding
            interactive: false
            currentIndex: mainTabBar.currentIndex

            Repeater {
                model: root.ctrl.openRepositoryModel

                RepositoryView {
                    repository: model.repository
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

        RoundButton {
            id: magicButton
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: Const.mainPadding * 2
            anchors.rightMargin: Const.mainPadding * 4
            text: currentText
            icon.source: currentIconSource

            Material.elevation: Const.popupElevation

            property bool mayCommit: root.ctrl.currentRepository
                                     ? root.ctrl.currentRepository.can(CEnums.Commit) && root.ctrl.currentRepository.hasCommitableFiles
                                     : false

            property bool mayPush: root.ctrl.currentRepository
                                   ? root.ctrl.currentRepository.can(CEnums.Push) && root.ctrl.currentRepository.hasPushableCommits
                                   : false

            property bool mayPull: root.ctrl.currentRepository
                                   ? root.ctrl.currentRepository.can(CEnums.Pull) && root.ctrl.currentRepository.hasPullableCommits
                                   : false

            property bool mayFetch: root.ctrl.currentRepository
                                    ? root.ctrl.currentRepository.can(CEnums.Fetch)
                                    : false

            property string currentIconSource: ""
            property string currentText: ""
            property Action currentAction: null

            onClicked: {
                if (currentAction)
                    currentAction.trigger()
            }

            onMayCommitChanged: checkMagicAction()
            onMayPushChanged: checkMagicAction()
            onMayPullChanged: checkMagicAction()
            onMayFetchChanged: checkMagicAction()

            function checkMagicAction() {
                visible = true

                if (mayCommit) {
                    currentText = Const.commitText
                    currentIconSource = Const.commitIcon
                    currentAction = menu.commitAction
                } else if (mayPush) {
                    currentText = Const.pushText
                    currentIconSource = Const.pushIcon
                    currentAction = menu.pushAction
                } else if (mayPull) {
                    currentText = Const.pullText
                    currentIconSource = Const.pullIcon
                    currentAction = menu.pullAction
                } else  if (mayFetch) {
                    currentText = Const.fetchText
                    currentIconSource = Const.fetchIcon
                    currentAction = menu.fetchAction
                } else {
                    visible = false
                }
            }
        }
    }

    //--------------------------------------------------------------------------------
    // Status bar

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

    //--------------------------------------------------------------------------------
    // Popups

    ClonePopup {
        id: cloneDialog
        anchors.centerIn: parent

        controller: root.ctrl

        onCloneBegins: statusTextHistory.open()
    }

    HelpPopup {
        id: helpDialog
        anchors.centerIn: parent

        controller: root.ctrl
    }

    StandardPopup {
        id: statusTextHistory
        anchors.centerIn: parent
        width: root.width * Const.popupWidthNorm

        title: Const.statusHistoryText

        controls: [
            StandardStringListView {
                width: parent.width
                height: Const.elementHeight * 20
                model: root.ctrl.statusTextHistory
                autoScrollToEnd: true
            }
        ]
    }

    QLP.FolderDialog {
        id: openDialog

        onAccepted: {
            root.ctrl.openRepository(folder)
        }
    }

    //--------------------------------------------------------------------------------
    // Shortcuts

    Shortcut {
        sequence: "Alt+Shift+F"
        onActivated: {
            if (currentRepositoryView)
                currentRepositoryView.activateFlatFileView()
        }
    }

    Shortcut {
        sequence: "Alt+Shift+G"
        onActivated: {
            if (currentRepositoryView)
                currentRepositoryView.activateGraphView()
        }
    }

    Shortcut {
        sequence: "Alt+Shift+L"
        onActivated: {
            if (currentRepositoryView)
                currentRepositoryView.activateBranchLogView()
        }
    }

    Shortcut {
        sequence: "Alt+Shift+D"
        onActivated: {
            if (currentRepositoryView)
                currentRepositoryView.activateFileDiffView()
        }
    }

    //--------------------------------------------------------------------------------
    // Functions

    function setTheme(theme) {
        Material.theme = theme
        ctrl.theme = (theme === Material.Dark ? Const.themeMaterialDark : Const.themeMaterialLight)
    }

    //--------------------------------------------------------------------------------
    // Debug

//    ScatteredItemView {
//        anchors.fill: parent
//        targetItem: parent
//    }
}
