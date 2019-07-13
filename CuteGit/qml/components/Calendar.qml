// Original file : ekke (Ekkehard Gentz) @ekkescorner
import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import Qt.labs.calendar 1.0
import QtQuick.Controls.Material 2.12

Popup {
    id: root
    parent: popupContainer
    anchors.centerIn: parent
    closePolicy: Popup.CloseOnPressOutside

    property var popupContainer: null
    property bool isLandscape: false
    property date selectedDate: new Date()
    property int displayMonth: selectedDate.getMonth()
    property int displayYear: selectedDate.getFullYear()
    property int calendarWidth: width * 0.9
    property int calendarHeight: height * 0.7
    property int itemPadding: width * 0.05
    property int itemSpacing: 2

    signal dateSelected()

    onOpened: {
        timeCombo.reset()
    }

    onSelectedDateChanged: {
//        console.log(
//                    "selectedDate",
//                    root.selectedDate.getFullYear(),
//                    root.selectedDate.getMonth(),
//                    root.selectedDate.getDate(),
//                    root.selectedDate.getHours(),
//                    root.selectedDate.getMinutes()
//                    )
    }

    GridLayout {
        id: calendarGrid
        width: root.calendarWidth
        height: root.calendarHeight
        // column 0 only visible if Landscape
        columns: 3
        // row 0 only visible if Portrait
        rows: 3

        ColumnLayout {
            id: title

            Layout.columnSpan: 2
            Layout.column: 1
            Layout.row: 0
            Layout.fillWidth: true

            spacing: itemSpacing

            RowLayout {
                height: implicitHeight * 2
                spacing: itemSpacing

                StandardButton {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    text: "<"

                    onClicked: {
                        if (root.displayMonth > 0) {
                            root.displayMonth --
                        } else {
                            root.displayMonth = 11
                            root.displayYear --
                        }
                    }
                }

                Label {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 3
                    text: monthGrid.title
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                StandardButton {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    text: ">"

                    onClicked: {
                        if (root.displayMonth < 11) {
                            root.displayMonth ++
                        } else {
                            root.displayMonth = 0
                            root.displayYear ++
                        }
                    }
                }
            }
        }

        // TODO not working in dark theme
        DayOfWeekRow {
            id: dayOfWeekRow

            Layout.column: 2
            Layout.row: 1
            Layout.fillWidth: true

            rightPadding: itemPadding
            font.bold: false

            delegate: Label {
                text: model.shortName
                font: dayOfWeekRow.font
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        // TODO not working in dark theme
        WeekNumberColumn {
            id: weekNumbers

            Layout.column: 1
            Layout.row: 2
            Layout.fillHeight: true

            leftPadding: itemPadding
            font.bold: false
            month: root.displayMonth
            year: root.displayYear

            delegate: Label {
                text: model.weekNumber
                font: weekNumbers.font
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        MonthGrid {
            id: monthGrid

            Layout.column: 2
            Layout.row: 2
            Layout.fillWidth: true
            Layout.fillHeight: true

            rightPadding: itemPadding

            month: root.displayMonth
            year: root.displayYear

            // ATTENTION: on Qt 5.9 clicked signal only if clicked with mouse
            // no event if tapped on a day
            // https://bugreports.qt.io/browse/QTBUG-61585
            // fixed in 5.9.2
            // so as a woraround I added a MouseArea for the delegate Label
            //            onClicked: {
            //                // Important: check the month to avoid clicking on days outside where opacity 0
            //                if(date.getMonth() == datePickerRoot.displayMonth) {
            //                    datePickerRoot.selectedDate = date
            //                    console.log("tapped on a date ")
            //                } else {
            //                    console.log("outside valid month "+date.getMonth())
            //                }
            //            }

            delegate: StandardLabel {
                id: dayLabel

                readonly property bool selected: model.day === root.selectedDate.getDate()
                                                 && model.month === root.selectedDate.getMonth()
                                                 && model.year === root.selectedDate.getFullYear()

                text: model.day
                font.bold: model.today? true: false
                opacity: model.month === monthGrid.month ? 1 : 0
                color: pressed || selected ? Material.foreground : model.today ? Material.accent : Material.foreground
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                background: Rectangle {
                    anchors.centerIn: parent
                    width: Math.min(parent.width, parent.height) * 1.2
                    height: width
                    radius: width / 2
                    visible: pressed || parent.selected
                    color: Material.primary
                }

                // WORKAROUND !! see onClicked()
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // Important: check the month to avoid clicking on days outside where opacity 0
                        if (date.getMonth() === root.displayMonth) {
                            var time = timeCombo.getTime()
                            var newDate = new Date()

                            newDate.setFullYear(date.getFullYear())
                            newDate.setMonth(date.getMonth())
                            newDate.setDate(date.getDate())
                            newDate.setHours(time.getHours())
                            newDate.setMinutes(time.getMinutes())
                            newDate.setSeconds(0)
                            newDate.setMilliseconds(0)

                            root.selectedDate = newDate
                            //                            console.log("tapped on a date ")
                        } else {
                            //                            console.log("outside valid month "+date.getMonth())
                        }
                    }
                }
            }
        }
    }

    RowLayout {
        id: timeLayout
        width: parent.width
        anchors.top: calendarGrid.bottom

        StandardLabel {
            Layout.alignment: Qt.AlignCenter
            text: Theme.timeText
        }

        ComboBox {
            id: timeCombo
            Layout.alignment: Qt.AlignCenter
            textRole: "text"

            model: ListModel {
                Component.onCompleted: {
                    for (var hour = 0; hour < 24; hour++) {
                        var hourText = hour < 10 ? "0" + hour : "" + hour

                        for (var minute = 0; minute < 60; minute += 15) {
                            var text = hourText + ":" + (minute < 10 ? "0" + minute : "" + minute)

                            append( { text: text, hour: hour - 1, minute: minute } )
                        }
                    }
                }
            }

            onCurrentIndexChanged: {
                root.selectedDate = getTime()
            }

            function reset() {
                currentIndex = timeToIndex(root.selectedDate)
            }

            function timeToIndex(date) {
                var hour = date.getHours()
                var minute = date.getMinutes()

                if (hour >= 22 && minute > 45) {
//                        console.log("timeToIndex() index:", 0)
                    return model.count - 1
                }

                for (var index = 0; index < model.count; index++) {
                    var element = model.get(index)

                    if  ((minute > 45 && element.hour > hour) || (element.hour >= hour && element.minute >= minute)) {
//                            console.log("timeToIndex() index:", index, " / ", element.hour, hour, element.minute, minute)
                        return index
                    }
                }

//                    console.log("timeToIndex() index:", -1)
                return -1
            }

            function getTime() {
                var newDate = new Date()
                var element = model.get(currentIndex)

                if (element !== null) {
                    newDate.setFullYear(root.selectedDate.getFullYear())
                    newDate.setMonth(root.selectedDate.getMonth())
                    newDate.setDate(root.selectedDate.getDate())
                    newDate.setHours(element.hour)
                    newDate.setMinutes(element.minute)
                    newDate.setSeconds(0)
                    newDate.setMilliseconds(0)
                }

                return newDate
            }
        }
    }

    RowLayout {
        width: parent.width
        anchors.top : timeLayout.bottom

        StandardButton {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("Cancel")

            onClicked: {
                root.close()
            }
        }

        StandardButton {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("Today")

            onClicked: {
                var hours = root.selectedDate.getHours()
                var minutes = root.selectedDate.getMinutes()

                var newDate = new Date()
                newDate.setHours(hours)
                newDate.setMinutes(minutes)
                newDate.setSeconds(0)
                newDate.setMilliseconds(0)

                root.selectedDate = newDate

                root.displayMonth = root.selectedDate.getMonth()
                root.displayYear = root.selectedDate.getFullYear()
            }
        }

        StandardButton {
            Layout.alignment: Qt.AlignCenter
            text: qsTr("OK")

            onClicked: {
                root.close()
                root.dateSelected()
            }
        }
    }
}
