import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4 as QC14
import QtQuick.Controls.Material 2.12
import "../components"
import "../views"
import "../popups"

Pane {
    id: root
    anchors.margins: Const.paneMargins

    Material.elevation: Const.paneElevation

    property variant repository: null

    QC14.TabView {
        anchors.fill: parent
        style: StandardTabViewStyle { }

        QC14.Tab {
            title: qsTr("Diff")

            DiffView {
                repository: root.repository
            }
        }

        QC14.Tab {
            title: qsTr("Log")

            FileLogPane {
                repository: root.repository
            }
        }
    }
}
