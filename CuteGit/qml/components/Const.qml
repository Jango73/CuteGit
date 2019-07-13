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

    // Margins and padding
    property int mainPadding: 15
    property int paneMargins: 8
    property int paneElevation: 2

    property int elementHeight: 30

    // Animations
    property int menuAnimationDuration: 500

    // Images

    // Text
    property string okText: qsTr("OK")
    property string cancelText: qsTr("Cancel")
}
