import QtQuick 2.12
import QtQuick.Controls 1.4 as QC14
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.4
import "../components"

Pane {
    id: root
    anchors.margins: Const.paneMargins

    Material.elevation: Const.paneElevation

    property variant controller: null

    QC14.TabView {
        anchors.fill: parent

        QC14.Tab {
            title: "Diff"

            DiffView {
                controller: root.controller
            }
        }

        QC14.Tab {
            title: "Log"

            LogView {
                controller: root.controller
            }
        }

        QC14.Tab {
            title: "Tool C"
        }

        style: TabViewStyle {
            frameOverlap: 1

            tab: Rectangle {
                color: styleData.selected ? Material.primary : Material.background
                implicitWidth: Math.max(text.width + 4, 80)
                implicitHeight: Const.elementHeight
                radius: Const.mainRadius

                FocusIndicator {
                    anchors.fill: parent
                    visible: styleData.activeFocus
                }

                StandardText {
                    id: text
                    anchors.centerIn: parent
                    text: styleData.title
                    color: styleData.selected ? Material.background : Material.foreground
                }
            }

            frame: Rectangle {
                color: Material.background
            }
        }
    }
}
