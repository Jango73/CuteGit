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
    }
}
