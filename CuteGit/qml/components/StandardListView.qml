import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

ListView {
    id: root
    interactive: true
    clip: true

    property bool autoScrollToEnd: false

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
    }
}
