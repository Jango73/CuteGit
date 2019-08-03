import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

ElideText {
    id: root

    property Item selection: null

    color: selection.show ? Material.background : Material.foreground
}
