import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"

StandardPopup {
    id: root

    property variant controller: null

    contentItem: Item {
        anchors.fill: parent

        StandardText {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Const.mainPadding

            horizontalAlignment: Text.AlignHCenter
            text: Const.cloneRepositoryText
        }

        TitleTextField {
            id: repositoryURL
            anchors.top: title.bottom
            width: parent.width
            title: Const.repositoryURLText
        }

        FolderSelector {
            id: repositoryPath
            anchors.top: repositoryURL.bottom
            width: parent.width
            title: Const.destinationFolderText
        }

        StandardToolBar {
            id: buttons
            width: parent.width
            height: cancelButton.height + Const.mainPadding
            anchors.bottom: parent.bottom

            Row {
                spacing: Const.mainPadding

                ToolButton {
                    action: Action {
                        id: okButton
                        text: Const.okText
                        onTriggered: {
                            root.controller.cloneRepository(repositoryURL.text, repositoryPath.text)
                            root.close()
                        }
                    }
                }

                ToolButton {
                    action: Action {
                        id: cancelButton
                        text: Const.cancelText
                        onTriggered: {
                            root.close()
                        }
                    }
                }
            }
        }
    }

    onOpened: {
        repositoryURL.text = root.controller.lastBrowsedRepositoryURL
        repositoryPath.text = root.controller.lastBrowsedRepositoryPath
    }

    onClosed: {
        root.controller.lastBrowsedRepositoryURL = repositoryURL.text
        root.controller.lastBrowsedRepositoryPath = repositoryPath.text
    }
}
