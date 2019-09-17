import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

ListView {
    id: root
    clip: true
    activeFocusOnTab: true
    contentWidth: width - scrollBarWidth

    /*! If true, the items can be selected */
    property bool itemsSelectable: true

    /*! If true, the list will automatically scroll to end when items are added */
    property bool autoScrollToEnd: false

    /*! The size of the scroll bar */
    property int scrollBarWidth: Const.mainPadding * 2

    signal spacePressed()
    signal enterPressed()
    signal deletePressed()

    ScrollBar.vertical: ScrollBar {
        width: scrollBarWidth
        active: true;
        minimumSize: (Const.mainPadding * 2) / height

        onActiveChanged: {
            if (!active)
                active = true;
        }
    }

    Timer {
        id: positionTimer
        interval: 100
        repeat: false
        onTriggered: root.positionViewAtIndex(root.count - 1, ListView.Beginning)
    }

    onCountChanged: {
        if (autoScrollToEnd)
            positionTimer.start()

        if (count > 0 && currentIndex == -1)
            currentIndex = 0
    }

    onCurrentIndexChanged: {
        if (!root.itemsSelectable)
            root.positionViewAtIndex(root.currentIndex, ListView.Beginning)
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_PageUp) {
            root.currentIndex = root.currentIndex - 5
        }
        else if (event.key === Qt.Key_PageDown) {
            root.currentIndex = root.currentIndex + 5
        }
        else if (event.key === Qt.Key_Home) {
            root.currentIndex = 0
        }
        else if (event.key === Qt.Key_End) {
            root.currentIndex = root.count - 1
        }
        else if (event.key === Qt.Key_Space) {
            root.spacePressed()
        }
        else if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            root.enterPressed()
        }
        else if (event.key === Qt.Key_Delete) {
            root.deletePressed()
        }
    }
}
