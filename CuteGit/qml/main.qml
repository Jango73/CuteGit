import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import "pages"

ApplicationWindow {
    id: app
    width: 1024
    height: 768
    visible: true

    Material.theme: Material.Dark
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
