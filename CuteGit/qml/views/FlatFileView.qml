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

    signal requestMenu(var name)
    signal requestDeleteFile(var name)

    model: root.repository
           ? root.repository.flatFileModelProxy
           : undefined

    delegate: StandardListViewItem {
        width: parent.width
        expanded: mustShowRelativeName
        selectionFillsItem: false
        listView: parent
        mouseEnabled: root.mouseActive
        symbolText: model.status
        primaryText: model.fileName
        secondaryText: mustShowRelativeName ? model.relativeName : ""
        selectionShown: selected
        focusShown: root.activeFocus && index === root.currentIndex

        property string fullName: model.fullName
        property bool mustShowRelativeName: model.fileName !== model.relativeName
        property bool selected: root.selection && root.modelIndices[index]
                                ? root.selection.hasSelection && root.selection.isSelected(root.modelIndices[index])
                                : false

        onPressed: {
            if (mouse.modifiers & Qt.ShiftModifier) {
                root.selectTo(index)
            } else {
                root.selectOnly(index)
            }

            root.currentIndex = index
            root.forceActiveFocus()
        }

        onClicked: {
            if (mouse.button === Qt.RightButton) {
                root.requestMenu(model.fullName)
            }
        }

        onDoubleClicked: root.repository.openFile(model.fullName)

        onFullNameChanged: root.modelIndices[index] = root.model.index(index, 0)

        Component.onCompleted: root.modelIndices[index] = root.model.index(index, 0)

        background: Rectangle {
            anchors.fill: parent
            color: if (model.staged === Const.staged) Const.fileStagedColor
                   else if (model.status === Const.statusModified) Const.fileModifiedColor
                   else if (model.status === Const.statusRenamed) Const.fileRenamedColor
                   else if (model.status === Const.statusAdded) Const.fileAddedColor
                   else if (model.status === Const.statusDeleted) Const.fileDeletedColor
                   else Const.transparent
        }
    }

    onCurrentIndexChanged: {
        if (root.selection && root.currentIndex !== -1 && typeof root.modelIndices[currentIndex] !== "undefined")
            root.selection.setCurrentIndex(root.modelIndices[currentIndex], ItemSelectionModel.Current)
    }

    onSpacePressed: root.repository.toggleStaged(currentItem.fullName)

    onDeletePressed: root.requestDeleteFile(currentItem.fullName)

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
