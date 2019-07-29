import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Qt.labs.platform 1.1 as QLP

Item {
    id: root
    anchors.margins: Const.mainPadding
    implicitHeight: Const.elementHeight * 2

    property alias title: title.text
    property alias text: folderName.text

    StandardLabel {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height * 0.5
    }

    Item {
        id: content
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        StandardTextField {
            id: folderName
            anchors.left: parent.left
            anchors.right: okButton.left
        }

        StandardButton {
            id: okButton
            text: Const.browseText
            anchors.right: parent.right

            onClicked: {
                pathDialog.open()
            }
        }
    }

    QLP.FolderDialog {
        id: pathDialog

        onAccepted: {
            folderName.text = folder
        }
    }
}
