import QtQuick 2.11
import QtQuick.Window 2.11
import GameBoardModel 1.0

GridView {
	id: root
    cellWidth: root.width / root.model.dimension
    cellHeight: root.height / root.model.dimension
    //highlight: Rectangle { color: "lightsteelblue"; radius: 10 }


    model: GameBoardModel {
        onSigGameWon: {
            wonTxt.visible = true
        }
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
        visible: false

        MouseArea {
            anchors.fill: wonTxt

            onClicked: {
                wonTxt.visible = false
                mouse.accepted = true
            }
        }
    }

}

//1 2 3
//  5 6
//7 8 9
