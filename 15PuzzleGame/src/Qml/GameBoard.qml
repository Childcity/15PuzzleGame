import QtQuick 2.11
import QtQuick.Window 2.11
import GameBoardModel 1.0
import AppSettings 1.0

GridView {
	id: root
    interactive: false
    cellWidth: root.width / boardModel.dimension
    cellHeight: root.height / boardModel.dimension

    model: GameBoardModel {
        id: boardModel
        dimension: AppSettings.dimension
        imageProvider: AppSettings.imageProvider
        onSigGameBoardError: {
            // arg1: errorString
            if (errorString === "TimeoutError") {
                errorPopup.errMsg = "Can't download image: " + errorString;
            } else if (errorString.length > 0) {
                errorPopup.errMsg = errorString
            }

            errorPopup.visible = true;
        }
    }

    delegate: Item {
        id: backgroundDelegate
        width: root.cellWidth
        height: root.cellHeight
        visible: index !== boardModel.hiddenIndex

        Tile {
            anchors.fill: backgroundDelegate
            anchors.margins: 3
            tileText: model.display.Value
            tileImg: model.display.Image

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    boardModel.move(index)
                }
            }

        }
    }

    GameWonPopup {
        id: wonPopup
        width: root.width - root.width / 8
        height: root.height - root.height / 2
        visible: boardModel.isGameWon

        onAboutToHide: {
            boardModel.resetBoard()
        }
    }

    ErrorMsgPopup {
        id: errorPopup
        width: root.width - root.width / 3
        height: root.height - root.height / 1.4
        visible: false

        onAboutToHide: {
            boardModel.resetBoard()
        }
    }

    Component.onCompleted: {
        if (! AppSettings.isValid()) {
            errorPopup.errMsg = "Settings can't be saved / loaded...\nError: " + AppSettings.status()
            errorPopup.visible = true
        }
    }

}
