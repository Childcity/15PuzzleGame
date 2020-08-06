import QtQuick 2.11
import QtQuick.Window 2.11
import GameBoardModel 1.0

GridView {
	id: root
    cellWidth: root.width / root.model.dimension
    cellHeight: root.height / root.model.dimension
    //highlight: Rectangle { color: "lightsteelblue"; radius: 10 }


    model: GameBoardModel {
    }

    delegate: Item {
        id: backgroundDelegate
        width: root.cellWidth
        height: root.cellHeight
        visible: index !== root.model.hiddenIndex

        Tile {
            anchors.fill: backgroundDelegate
            anchors.margins: 5
            tileText: model.display
        }
    }
    MouseArea {
        anchors.fill: root
        onClicked: {
            console.log(root.model.dimension)
            model.dimension = ++model.dimension
        }
    }
}
