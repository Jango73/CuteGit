import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "pages"

ApplicationWindow {
    id: root
    width: 1024
    height: 768
    visible: true

    property var ctrl: controller

    Material.theme: Material.Dark
    Material.primary: Material.Teal
    Material.accent: Material.Green

    MainPage {
        anchors.fill: parent
        controller: root.ctrl
    }
}
