import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    id: root
    height: setButton.height

    property var popupContainer: null
    property int labelsWidth: width - setButton.width
    property int labelsHeight: setButton.height
    property date selectedDate: null

    Component.onCompleted: {
        reset()
    }

    StandardText {
        id: dateDisplay
        anchors.right: timeDisplay.left
        width: labelsWidth * 0.5
        height: labelsHeight
        text: selectedDate.toLocaleDateString()

        verticalAlignment: Text.AlignVCenter
    }

    StandardText {
        id: timeDisplay
        anchors.right: setButton.left
        width: labelsWidth * 0.5
        height: labelsHeight
        text: selectedDate.toLocaleTimeString()

        verticalAlignment: Text.AlignVCenter
    }

    StandardButton {
        id: setButton
        anchors.right: parent.right
        text: Theme.setText

        onClicked: {
            calendar.selectedDate = root.selectedDate
            calendar.open()
        }
    }

    Calendar {
        id: calendar
        width: root.popupContainer.width * 0.9
        height: root.popupContainer.height * 0.7
        popupContainer: root.popupContainer

        onDateSelected: {
            root.selectedDate = selectedDate
        }
    }

    function reset() {
        var newDate = new Date()
        newDate.setSeconds(0)
        newDate.setMilliseconds(0)

        root.selectedDate = newDate
    }
}
