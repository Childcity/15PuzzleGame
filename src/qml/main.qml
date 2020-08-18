import QtQuick 2.11
import QtQuick.Window 2.11

Window {
    id: root
    title: qsTr("15 Puzzle")
    visible: true
    width: 300
    height: 300

    GameBoard {
        id: board
        anchors.fill: parent
    }

    Component.onCompleted: {
        root.x = Screen.width / 2 - root.width / 2
        root.y = Screen.height / 2 - root.height / 2
    }
}
