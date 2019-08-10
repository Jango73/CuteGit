import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import "../components"
import ".."

StandardPopup {
    id: root
    width: parent.width * Const.popupWidthNorm
    height: content.computedHeight + Const.mainPadding * 2

    property variant controller: null

    signal cloneBegins()

    contentItem: Item {
        id: content

        property int computedHeight: title.height + repositoryURL.height + repositoryPath.height + buttons.height

        Column {
            id: layout
            anchors.fill: parent

            StandardText {
                id: title
                width: parent.width
                height: Const.elementHeight * 2

                horizontalAlignment: Text.AlignHCenter
                text: Const.cloneRepositoryText
            }

            TitleTextField {
                id: repositoryURL
                width: parent.width
                height: implicitHeight

                title: Const.repositoryURLText
            }

            FolderSelector {
                id: repositoryPath
                width: parent.width
                height: implicitHeight

                title: Const.destinationFolderText
            }

            StandardToolBar {
                id: buttons
                width: parent.width
                height: implicitHeight

                Row {
                    spacing: Const.mainPadding

                    StandardToolButton {
                        action: Action {
                            id: okButton
                            text: Const.okText
                            onTriggered: {
                                root.cloneBegins()
                                root.controller.cloneRepository(repositoryURL.text, repositoryPath.text)
                                root.close()
                            }
                        }
                    }

                    StandardToolButton {
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
