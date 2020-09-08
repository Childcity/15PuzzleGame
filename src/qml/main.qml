import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3

ApplicationWindow {
    id: root
    title: qsTr("15 Puzzle")
    visible: true
    width: 600
    height: 600

    background: Rectangle {
        color: "darkGray"
    }

    header: MainMenuBar {
    }

    GameBoard {
        id: board
        anchors.fill: root.contentItem
        anchors.margins: 5
    }

    Component.onCompleted: {
        root.x = Screen.width / 2 - root.width / 2
        root.y = Screen.height / 2 - root.height / 2
    }
}
