import QtQuick 2.12
import QtQuick.Layouts 1.3

StackLayout {
    id: root

    property Item currentItem: children[currentIndex]
}
