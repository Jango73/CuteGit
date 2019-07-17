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
    property color fileStagedColor: "#080"
    property color fileModifiedColor: "#800"
    property color fileAddedColor: "#008"
    property color fileDeletedColor: "#840"

    // Margins and padding
    property int mainPadding: 16
    property int mainRadius: 2
    property int paneMargins: 8
    property int paneElevation: 2
    property int popupElevation: 8
    property int elementHeight: mainFontSize * 1.5
    property int buttonHeight: mainFontSize * 2.5
    property int treeElementHeight: mainFontSize * 1.3

    property real popupWidthNorm: 0.50
    property real popupHeightNorm: 0.75

    // Animations
    property int menuAnimationDuration: 500

    // Images

    // Text
    property string okText: qsTr("OK")
    property string clearText: qsTr("Clear")
    property string cancelText: qsTr("Cancel")
    property string outputText: qsTr("Output")
    property string expandAllText: qsTr("Expand all")
    property string collapseAllText: qsTr("Collapse all")
    property string repositoriesText: qsTr("Repositories")
    property string enterMessageHereText: qsTr("Enter your message here...")

    //
    property string statusModified: "*"
    property string statusAdded: "+"
    property string statusDeleted: "-"
}
