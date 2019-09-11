import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import CuteGit 1.0
import "../components"
import "../views"
import "../popups"

ExtendablePane {
    id: root
    title: qsTr("Files")

    property variant repository: null
    property variant flatSelection: null

    signal requestMenu(var name)
    signal requestDeleteFile(var name)
    signal requestFileFilter(var text)

    content: [
        Item {
            id: filter
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: filterText.height

            StandardLabel {
                id: filterLabel
                height: filterText.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("Filter")
            }

            StandardTextField {
                id: filterText
                anchors.left: filterLabel.right
                anchors.right: parent.right
                anchors.leftMargin: Const.mainPadding

                onTextChanged: {
                    root.requestFileFilter(text)
                }
            }
        },

        StandardLabel {
            anchors.fill: flatFileView
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: Const.allFilesCleanText
            visible: root.repository === null | flatFileView.count === 0
        },

        FlatFileView {
            id: flatFileView
            anchors.top: filter.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            repository: root.repository
            selection: root.flatSelection

            onRequestMenu: root.requestMenu(name)
            onRequestDeleteFile: root.requestDeleteFile(name)
        }
    ]

    function getSelectedFiles() {
        return flatFileView.model.selectionToFullNameList(flatSelection.selectedIndexes)
    }

    function activateFlatFileView() {
        flatFileView.forceActiveFocus()
    }
}
