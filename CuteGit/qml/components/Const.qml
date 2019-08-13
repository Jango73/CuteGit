pragma Singleton
import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.12

Item {
    // Constants
    property string statusOnline: "online"

    // Main font
    property string mainFontFamily: "Calibri"
    property int mainFontSize: Screen.desktopAvailableHeight / 65

    // Colors
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
    property int verySmallPadding: smallPadding / 2
    property int panePadding: smallPadding
    property int paneMargins: smallPadding
    property int paneElevation: 2
    property int popupElevation: 8
    property int elementHeight: mainFontSize * 1.5
    property int buttonHeight: mainFontSize * 2.5

    property real popupWidthNorm: 0.50
    property real popupHeightNorm: 0.75

    property real popupWidthSmall: 0.25
    property real popupHeightSmall: 0.25

    // Durations
    property int componentFadingDuration: 200

    // Text
    property string repositoryMenuText: qsTr("&Repository")
    property string cloneMenuText: qsTr("&Clone / checkout")
    property string openMenuText: qsTr("&Open")
    property string removeMenuText: qsTr("&Remove")
    property string knownMenuText: qsTr("&Known")
    property string quitMenuText: qsTr("&Quit")

    property string remoteMenuText: qsTr("Re&mote")
    property string fetchMenuText: qsTr("&Fetch")
    property string pullMenuText: qsTr("&Pull")
    property string pushMenuText: qsTr("Pus&h")

    property string okText: qsTr("OK")
    property string logText: qsTr("Log")
    property string tagsText: qsTr("Tags")
    property string flatText: qsTr("Flat")
    property string diffText: qsTr("Diff")
    property string clearText: qsTr("Clear")
    property string filesText: qsTr("Files")
    property string stageText: qsTr("Stage")
    property string mergeText: qsTr("Merge")
    property string graphText: qsTr("Graph")
    property string amendText: qsTr("Amend")
    property string revertText: qsTr("Revert")
    property string cancelText: qsTr("Cancel")
    property string outputText: qsTr("Output")
    property string browseText: qsTr("Browse")
    property string deleteText: qsTr("Delete")
    property string commitText: qsTr("Commit")
    property string generalText: qsTr("General")
    property string branchesText: qsTr("Branches")
    property string switchToText: qsTr("Switch to")
    property string amendingText: qsTr("Amending...")
    property string shortcutsText: qsTr("Shortcuts")
    property string branchLogText: qsTr("Branch log")
    property string helpTitleText: qsTr("Help")
    property string listEmptyText: qsTr("List empty")
    property string expandAllText: qsTr("Expand all")
    property string branchFromText: qsTr("Branch from")
    property string collapseAllText: qsTr("Collapse all")
    property string mergeBranchText: qsTr("Merge branch")
    property string deleteBranchText: qsTr("Delete branch")
    property string repositoriesText: qsTr("Repositories")
    property string repositoryURLText: qsTr("Repository URL")
    property string statusHistoryText: qsTr("Status history")
    property string mergeProgressText: qsTr("Merge in progress...")
    property string allFilesCleanText: qsTr("All files are clean")
    property string rebaseProgressText: qsTr("Rebase in progress...")
    property string cloneRepositoryText: qsTr("Clone repository")
    property string enterMessageHereText: qsTr("Enter your message here...")
    property string destinationFolderText: qsTr("Destination folder")
    property string enterBranchNameHereText: qsTr("Enter branch name here...")
    property string changeCommitMessageText: qsTr("Change commit message")
    property string stagedFilesForCommitText: qsTr("Staged files for commit")
    property string interactiveRebaseProgressText: qsTr("Interactive rebase in progress...")

    property string emptyRepositoryTabText: qsTr(
                                                "There is no open repository at the moment.\n" +
                                                "Please open or clone a repository to fill this view."
                                                )

    property string mergeBranchMessage: qsTr(
                                            "You are about to merge {0} on the working directory.\n\n" +
                                            "Are you sure you want to do this?"
                                            )

    property string deleteBranchMessage: qsTr(
                                             "You are about to delete a branch.\n\n" +
                                             "Are you sure you want to do this? It cannot be undone."
                                             )

    property string copyrightText: qsTr(
                                  "CuteGit - version {0}\n" +
                                  "Copyright (c) 2019 Jango73\n" +
                                  "Some icons are copyright Freepik from www.flaticon.com\n"
                                  )

    property string generalHelpText: qsTr(
                                         "File views\n" +
                                         "\n" +
                                         "Staged files are shown in green, unstaged ones in red.\n" +
                                         "The symbols next to file names are:\n" +
                                         "*\tThe file is modified\n" +
                                         "+\tThe file is being added\n" +
                                         "-\tThe file is being removed\n" +
                                         "=\tThe file is being renamed\n" +
                                         "?\tThe file is untracked\n" +
                                         "!\tThe file is ignored\n" +
                                         "???\tThe file is missing\n" +
                                         "\n" +
                                         "Diff views\n" +
                                         "...\n" +
                                         "\n" +
                                         "Log views\n" +
                                         "...\n"
                                         )

    property string shortcutHelpText: qsTr(
                                          "Shortcuts\n" +
                                          "\n" +
                                          "Open a repository\t\tControl O\n" +
                                          "Quit\t\t\tControl Q\n" +
                                          "\n" +
                                          "Refresh\t\t\tF5\n" +
                                          "\n" +
                                          "Stage all\t\t\tControl Shift +\n" +
                                          "Unstage all\t\t\tControl Shift -\n" +
                                          "Toggle staged\t\tSpace\n" +
                                          "Stage selection\t\tControl +\n" +
                                          "Unstage selection\t\tControl -\n" +
                                          "Revert selection\t\tControl Z\n" +
                                          "Commit\t\t\tControl C\n" +
                                          "Amend\t\t\tControl A\n" +
                                          "Continue rebase\t\tControl R\n" +
                                          "Abort rebase\t\tControl T\n" +
                                          "\n" +
                                          "Fetch\t\t\tControl F\n" +
                                          "Pull\t\t\tControl L\n" +
                                          "Push\t\t\tControl P\n" +
                                          "\n"
                                          )

    //
    property string statusModified: "*"
    property string statusAdded: "+"
    property string statusDeleted: "-"

    // Icons

    // By Freepik from www.flaticon.com
    property string cloneIcon: "qrc:/qml/images/copy-documents-option.svg"
    property string openIcon: "qrc:/qml/images/open-folder-black-and-white-variant"
    property string pushIcon: "qrc:/qml/images/up-arrow.svg"
    property string pullIcon: "qrc:/qml/images/down.svg"
    property string fetchIcon: "qrc:/qml/images/loop.svg"
    property string stageIcon: "qrc:/qml/images/check.svg"
    property string revertIcon: "qrc:/qml/images/garbage-container.svg"
    property string commitIcon: "qrc:/qml/images/seal.svg"
    property string amendIcon: "qrc:/qml/images/plus-sign.svg"
    property string saveStashIcon: "qrc:/qml/images/clipboard.svg"
    property string popStashIcon: "qrc:/qml/images/blank.svg"
    property string exitIcon: "qrc:/qml/images/door-exit.svg"
}
