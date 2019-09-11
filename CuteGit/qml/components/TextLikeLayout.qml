import QtQuick 2.12

Item {
    id: root
    implicitHeight: computedHeight

    property int spacing: 0
    property int computedHeight: 0

    onChildrenChanged: {
        if (children.length > 0) {
            var x = 0
            var y = 0
            var h = children[0].height

            for (var i = 0; i < children.length; i++)
            {
                // console.log("item "+i);
                // console.log(children[i].propname);

                children[i].x = x
                children[i].y = y

                if (x + children[i].width > width) {
                    y += children[i].height
                }
            }

            computedHeight = y + h
        } else {
            computedHeight = 0
        }
    }
}
