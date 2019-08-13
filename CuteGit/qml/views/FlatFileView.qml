import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQml.Models 2.2
import CuteGit 1.0
import "../components"

StandardListView {
    id: root

    property variant repository: null
    property variant selection: null
    property bool mouseActive: true
    property variant modelIndices: ({})

    model: root.repository
           ? root.repository.flatFileModelProxy
           : undefined

    delegate: Item {
        id: dlg
        width: parent.width
        height: Const.elementHeight + Const.mainPadding * 0.25

        property string fullName: model.fullName
        property bool selected: root.selection
                                ? root.selection.hasSelection && root.selection.isSelected(root.modelIndices[index])
                                : false

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            enabled: root.mouseActive
            hoverEnabled : true

            onPressed: {
                if (mouse.modifiers & Qt.ShiftModifier) {
                    root.selectTo(index)
                } else {
                    root.selectOnly(index)
                }

                root.currentIndex = index
                root.forceActiveFocus()
            }

            onDoubleClicked: {
                root.repository.openFile(model.fullName)
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
                width: parent.width - Const.mainPadding
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
                anchors.fill: listViewFileNameText
                show: dlg.selected
            }

            FocusIndicator {
                anchors.fill: parent
                visible: root.activeFocus && index === root.currentIndex
            }

            TextOverSelection {
                id: listViewFileNameText
                width: parent.width - Const.mainPadding
                anchors.centerIn: parent
                text: model.fileName

                selection: listSelection
            }
        }

        Item {
            id: listViewRelativeName
            anchors.left: listViewFileName.right
            anchors.right: parent.right
            height: parent.height

            ElideText {
                id: listViewRelativeNameText
                width: parent.width - Const.mainPadding
                anchors.centerIn: parent
                color: Material.foreground
                text: model.relativeName
            }
        }

        Component.onCompleted: root.modelIndices[index] = root.model.index(index, 0)
        onFullNameChanged: root.modelIndices[index] = root.model.index(index, 0)
    }

    onCurrentIndexChanged: {
        if (root.currentIndex !== -1 && root.selection)
            root.selection.setCurrentIndex(root.modelIndices[currentIndex], ItemSelectionModel.Current)
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Space) {
            root.repository.toggleStaged(currentItem.fullName)
        }
    }

    Keys.onReleased: {
    }

    function selectTo(targetIndex) {
        var selIndex
        selection.clear()

        if (targetIndex === root.currentIndex) {
            root.selection.select(root.modelIndices[targetIndex], ItemSelectionModel.ToggleCurrent)
        }
        else if (targetIndex > root.currentIndex) {
            for (selIndex = root.currentIndex; selIndex <= targetIndex; selIndex++) {
                root.selection.select(root.modelIndices[selIndex], ItemSelectionModel.Select)
            }
        }
        else {
            for (selIndex = root.currentIndex; selIndex >= targetIndex; selIndex--) {
                root.selection.select(root.modelIndices[selIndex], ItemSelectionModel.Select)
            }
        }
    }

    function selectOnly(targetIndex) {
        selection.clear()
        root.selection.select(root.modelIndices[targetIndex], ItemSelectionModel.SelectCurrent)
    }
}
