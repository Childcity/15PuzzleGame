import QtQuick 2.11
import QtQuick.Window 2.11

GridView {
	id: root
    cellWidth: root.width / Math.sqrt(el.count)
    cellHeight: root.height / Math.sqrt(el.count)

	
    model: ListModel {
        id: el
        ListElement {
            display: "1"
        }
        ListElement {
            display: "2"
        }
        ListElement {
            display: "3"
        }
        ListElement {
            display: "4"
        }
    }

    delegate: Item {
        id: backgroundDelegate
        width: root.cellWidth
        height: root.cellHeight

        Tile {
            anchors.fill: backgroundDelegate
            anchors.margins: 5
            tileText: model.display
        }
    }
}
