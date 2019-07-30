import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"

StandardListView {
    id: root

    property variant repository: null
    property bool mouseActive: true

    model: root.repository !== null ? root.repository.flatFileModelProxy : undefined

    onCurrentIndexChanged: root.repository.flatFileModelProxy.handleCurrentIndex(currentModelIndex())

    delegate: Item {
        width: parent.width
        height: Const.treeElementHeight + Const.mainPadding * 0.25

        property string fullName: model.fullName

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            enabled: root.mouseActive
            onClicked: {
                root.currentIndex = index
                root.forceActiveFocus()
            }
        }

        Rectangle {
            anchors.fill: parent
            color: if (model.staged === "X") Const.fileStagedColor
                   else if (model.status === "*") Const.fileModifiedColor
                   else if (model.status === "=") Const.fileRenamedColor
                   else if (model.status === "+") Const.fileAddedColor
                   else if (model.status === "-") Const.fileDeletedColor
                   else Const.transparent
        }

        Item {
            id: listViewStatus
            width: Const.elementHeight
            height: parent.height
            anchors.left: parent.left

            ElideText {
                id: listViewStatusText
                width: parent.width - Const.smallPadding
                anchors.centerIn: parent
                color: Material.foreground
                text: model.status
            }
        }

        Item {
            id: listViewFileName
            anchors.left: listViewStatus.right
            width: parent.width * 0.4
            height: parent.height

            Selection {
                id: listSelection
                targetWidth: listViewFileNameText.width
                targetHeight: listViewFileNameText.height
                anchors.centerIn: listViewFileNameText
                borderOnly: true
                visible: index === root.currentIndex

                FocusIndicator {
                    anchors.fill: parent
                    visible: root.activeFocus
                }
            }

            ElideText {
                id: listViewFileNameText
                width: parent.width - Const.smallPadding
                anchors.centerIn: parent
                color: Material.foreground
                text: model.fileName
            }
        }

        Item {
            id: listViewRelativeName
            anchors.left: listViewFileName.right
            anchors.right: parent.right
            height: parent.height

            ElideText {
                id: listViewRelativeNameText
                width: parent.width - Const.smallPadding
                anchors.centerIn: parent
                color: Material.foreground
                text: model.relativeName
            }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Space) {
            root.repository.toggleStaged(currentItem.fullName)
        }
    }

    function currentModelIndex() {
        return root.model.index(currentIndex, 0)
    }
}
