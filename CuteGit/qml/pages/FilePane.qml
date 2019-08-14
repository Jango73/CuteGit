import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"
import "../views"
import "../popups"

Pane {
    id: root
    padding: Const.mainPadding

    Material.elevation: Const.paneElevation

    property variant repository: null
    property variant flatSelection: null

    signal requestDeleteFile(var name)

    StandardLabel {
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: Const.allFilesCleanText
        visible: root.repository === null | flatFileView.count === 0
    }

    FlatFileView {
        id: flatFileView
        anchors.fill: parent

        repository: root.repository
        selection: root.flatSelection

        onRequestDeleteFile: root.requestDeleteFile(name)
    }

    function getSelectedFiles() {
        return flatFileView.model.selectionToFullNameList(flatSelection.selectedIndexes)
    }

    function activateFlatFileView() {
        flatFileView.forceActiveFocus()
    }
}
