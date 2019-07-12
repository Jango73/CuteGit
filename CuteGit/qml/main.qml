import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0
import Qt.labs.settings 1.0
import CuteGit 1.0
import "components"
import "pages"

ApplicationWindow {
    id: app
    width: 1024
    height: 768
    visible: true

    Material.primary: Material.Teal
    Material.accent: Material.Green

    MainPage {
        anchors.fill: parent
        fileModel: controller.fileModel
        repositoryModel: controller.repositoryModel
        repositoryPath: controller.repositoryPath

        onRepositorySelected: {
            controller.setRepository(path)
        }
    }
}
