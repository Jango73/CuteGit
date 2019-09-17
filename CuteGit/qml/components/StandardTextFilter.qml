import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root
    height: filterText.height

    property alias text: filterLabel.text

    signal filterTextChanged(var text)

    StandardLabel {
        id: filterLabel
        height: filterText.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    StandardTextField {
        id: filterText
        anchors.left: filterLabel.right
        anchors.right: parent.right
        anchors.leftMargin: Const.mainPadding

        onTextChanged: {
            root.filterTextChanged(text)
        }
    }
}
