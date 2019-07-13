pragma Singleton
import QtQuick 2.12
import QtQuick.Controls.Material 2.12

Item {
    // Constants
    property string statusOnline: "online"

    // Main font
    property string mainFontFamily: "Calibri"
    property int mainFontSize: 16

    // Colors
    property color foregroundLighter: "gray"
    property color transparent: "transparent"

    // Margins and padding
    property int mainPadding: 16
    property int paneMargins: 8
    property int paneElevation: 2
    property int popupElevation: 8
    property int elementHeight: 30

    property real popupWidthNorm: 0.50
    property real popupHeightNorm: 0.75

    // Animations
    property int menuAnimationDuration: 500

    // Images

    // Text
    property string okText: qsTr("OK")
    property string cancelText: qsTr("Cancel")
    property string expandAllText: qsTr("Expand all")
    property string collapseAllText: qsTr("Collapse all")
    property string enterMessageHereText: qsTr("Enter your message here...")
}
