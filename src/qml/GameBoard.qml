import QtQuick 2.11
import QtQuick.Window 2.11
import GameBoardModel 1.0

GridView {
	id: root
    interactive: false
    cellWidth: root.width / root.model.dimension
    cellHeight: root.height / root.model.dimension
    //highlight: Rectangle { color: "lightsteelblue"; radius: 10 }


    model: GameBoardModel {
        onSigGameBoardError: {
            // arg1: errorString
            if (errorString === "TimeoutError") {
                errorPopup.errMsg = "Can't download image: " + errorString;
            }

            errorPopup.visible = true;
        }
    }

    delegate: Item {
        id: backgroundDelegate
        width: root.cellWidth
        height: root.cellHeight
        visible: index !== root.model.hiddenIndex

        Tile {
            anchors.fill: backgroundDelegate
            anchors.margins: 3
            tileText: model.display.Value
            tileImg: model.display.Image

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.model.move(index)
                }
            }

        }
    }

    GameWonPopup {
        id: wonPopup
        width: root.width - root.width / 8
        height: root.height - root.height / 2
        visible: root.model.isGameWon

        onAboutToHide: {
            root.model.resetBoard()
        }
    }

    ErrorMsgPopup {
        id: errorPopup
        width: root.width - root.width / 3
        height: root.height - root.height / 1.4
        visible: false

        onAboutToHide: {
            root.model.resetBoard()
        }
    }

}
