import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root
    width: labelText.width

    property alias text: labelText.text

    Selection {
        anchors.centerIn: labelText
        targetWidth: labelText.width
        targetHeight: labelText.height
        color: Material.accent
        show: true
    }

    StandardText {
        id: labelText
        anchors.verticalCenter: parent.verticalCenter
        color: Material.background
    }
}
