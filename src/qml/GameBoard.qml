import QtQuick 2.11
import QtQuick.Window 2.11
import GameBoardModel 1.0

GridView {
	id: root
    cellWidth: root.width / root.model.dimension
    cellHeight: root.height / root.model.dimension
    //highlight: Rectangle { color: "lightsteelblue"; radius: 10 }


    model: GameBoardModel {
        //onSigGameWonChanged: {
        //    // arg1: isGameWon
        //    wonTxt.visible = isGameWon
        //}
    }

    delegate: Item {
        id: backgroundDelegate
        width: root.cellWidth
        height: root.cellHeight
        visible: index !== root.model.hiddenIndex

        Tile {
            anchors.fill: backgroundDelegate
            anchors.margins: 5
            tileText: {
                console.log(typeof( model.display.Value),typeof( model.display))
                model.display.Value
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.model.move(index)
                }
            }
        }
    }

    GameWon {
        id: wonTxt
        anchors.centerIn: root
        width: root.width - root.width / 8
        height: root.height - root.height / 2
        visible: root.model.isGameWon

        MouseArea {
            anchors.fill: wonTxt

            onClicked: {
                root.model.dimension++
                root.model.dimension--
                mouse.accepted = true
            }
        }
    }

}
