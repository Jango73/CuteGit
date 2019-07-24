import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import "../components"

TitlePane {
    id: root

    property variant controller: null

    title: Const.repositoriesText

    content: Item {
        anchors.fill: parent

        StandardLabel {
            id: listPlaceHolder
            anchors.fill: parent
            visible: !list.visible
            text: "Open a repository..."
        }

        StandardListView {
            id: list
            anchors.fill: parent
            visible: count > 0

            model: root.controller.repositoryModel

            delegate: Item {
                width: parent.width
                height: Const.elementHeight

                Item {
                    anchors.fill: parent
                    anchors.margins: Const.smallPadding

                    MouseArea {
                        anchors.fill: selection
                        onDoubleClicked: {
                            root.controller.repository.repositoryPath = display
                        }
                    }

                    Selection {
                        id: selection
                        targetWidth: text.width
                        targetHeight: text.height
                        anchors.centerIn: text
                        visible: display === root.controller.repository.repositoryPath
                    }

                    ElideText {
                        id: text
                        width: parent.width - Const.smallPadding
                        text: display.split("/").slice(-1)[0]
                        color: selection.visible ? Material.background : Material.foreground
                    }
                }
            }
        }
    }
}
