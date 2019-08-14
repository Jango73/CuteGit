import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

ListView {
    id: root
    clip: true
    activeFocusOnTab: true
    contentWidth: width - scrollBarWidth

    property bool autoScrollToEnd: false
    property int scrollBarWidth: Const.mainPadding * 2

    signal spacePressed()
    signal deletePressed()

    ScrollBar.vertical: ScrollBar {
        width: scrollBarWidth
        active: true;

        onActiveChanged: {
            if (!active)
                active = true;
        }
    }

    Timer {
        id: positionTimer
        interval: 100
        repeat: false
        onTriggered: root.positionViewAtIndex(root.count - 1, ListView.Visible)
    }

    onCountChanged: {
        if (autoScrollToEnd)
            positionTimer.start()

        if (count > 0 && currentIndex == -1)
            currentIndex = 0
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
        else if (event.key === Qt.Key_Delete) {
            root.deletePressed()
        }
    }
}
