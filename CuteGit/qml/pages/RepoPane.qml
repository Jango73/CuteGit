import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import "../components"

Pane {
    id: root
    anchors.margins: Const.paneMargins

    Material.elevation: Const.paneElevation

    property variant controller: null

    ListView {
        anchors.fill: parent
        clip: true
        interactive: true

        model: root.controller.repositoryModel

        delegate: Item {
            width: parent.width
            height: Const.elementHeight

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    controller.repositoryPath = display
                }
            }

            Selection {
                id: selection
                targetWidth: text.width
                targetHeight: text.height
                anchors.centerIn: text
                visible: display === root.controller.repositoryPath
            }

            StandardText {
                id: text
                text: display.split("/").slice(-1)[0]
                color: selection.visible ? Material.background : Material.foreground
            }
        }
    }
}
