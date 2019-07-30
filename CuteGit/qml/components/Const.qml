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
    property color fileStagedColor: "#80008000"
    property color fileModifiedColor: "#80800000"
    property color fileRenamedColor: "#80804000"
    property color fileAddedColor: "#80000080"
    property color fileDeletedColor: "#80804000"

    // Margins and padding
    property int mainPadding: 8
    property int mainRadius: 4
    property int smallPadding: mainPadding / 2
    property int paneMargins: smallPadding
    property int paneElevation: 2
    property int popupElevation: 8
    property int elementHeight: mainFontSize * 1.5
    property int buttonHeight: mainFontSize * 2.5
    property int treeElementHeight: mainFontSize * 1.3

    property real popupWidthNorm: 0.50
    property real popupHeightNorm: 0.75

    property real popupWidthSmall: 0.25
    property real popupHeightSmall: 0.25

    // Text
    property string okText: qsTr("OK")
    property string logText: qsTr("Log")
    property string tagsText: qsTr("Tags")
    property string clearText: qsTr("Clear")
    property string filesText: qsTr("Files")
    property string cancelText: qsTr("Cancel")
    property string outputText: qsTr("Output")
    property string browseText: qsTr("Browse")
    property string deleteText: qsTr("Delete")
    property string branchesText: qsTr("Branches")
    property string switchToText: qsTr("Switch to")
    property string amendingText: qsTr("Amending...")
    property string expandAllText: qsTr("Expand all")
    property string branchFromText: qsTr("Branch from")
    property string collapseAllText: qsTr("Collapse all")
    property string deleteBranchText: qsTr("Delete branch")
    property string repositoriesText: qsTr("Repositories")
    property string repositoryURLText: qsTr("Repository URL")
    property string cloneRepositoryText: qsTr("Clone repository")
    property string nothingToDisplayText: qsTr("Nothing to display")
    property string enterMessageHereText: qsTr("Enter your message here...")
    property string destinationFolderText: qsTr("Destination folder")
    property string enterBranchNameHereText: qsTr("Enter branch name here...")

    property string deleteBranchMessage: qsTr(
                                             "You are about to delete a branch.\n\n" +
                                             "Are you sure you want to do this? It cannot be undone."
                                             )

    property string shortcutsText: qsTr(
                                       "Shortcuts\n" +
                                       "Open a repository\t\tControl + 'O'\n" +
                                       "Quit\t\t\tControl + 'Q'\n" +
                                       "\n" +
                                       "Refresh\t\t\tF5\n" +
                                       "\n" +
                                       "Stage all\t\t\tControl + Shift + '+'\n" +
                                       "Unstage all\t\t\tControl + Shift + '-'\n" +
                                       "Toggle staged\t\tSpace\n" +
                                       "Stage selection\t\tControl + '+'\n" +
                                       "Unstage selection\t\tControl + '-'\n" +
                                       "Revert selection\t\tControl + 'Z'\n" +
                                       "Commit\t\t\tControl + 'C'\n" +
                                       "Amend\t\t\tControl + 'A'\n" +
                                       "Continue rebase\t\tControl + 'R'\n" +
                                       "Abort rebase\t\tControl + 'T'\n" +
                                       "\n" +
                                       "Fetch\t\t\tControl + 'F'\n" +
                                       "Pull\t\t\tControl + 'L'\n" +
                                       "Push\t\t\tControl + 'P'\n"
                                       )

    //
    property string statusModified: "*"
    property string statusAdded: "+"
    property string statusDeleted: "-"
}
