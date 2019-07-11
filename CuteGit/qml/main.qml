import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import Qt.labs.settings 1.0
import CuteGit 1.0
import "components"
import "pages"

// Main application window
ApplicationWindow {
    id: app
    width: 1024
    height: 768

    Material.primary: Material.Teal
    Material.accent: Material.Green

    // Set visible
    visible: true

    MainPage {
        anchors.fill: parent
        model: controller.fileModel
    }
}
