import QtQuick 2.12
import QtQuick.Controls 2.5
import "."

StandardListView {
    delegate: ElideText {
        width: parent.width
        height: Const.listViewItemHeight
        text: display
    }
}
