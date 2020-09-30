import QtQuick 2.11
import "CustomControls" as Custom

Rectangle {
    id: root
    clip: true
    radius: 5
    //color: "#bff"r
	border {
		color: "black"
		width: 1
    }

    property string tileText: "0"
    property string tileTextColor: "white"
    property string tileImg: ""

    Image {
        id: img
        source: {
            if (tileImg.length === 0) {
                img.visible = false
                loadingImg.visible = true
            } else {
                img.visible = true
                loadingImg.visible = false
            }
            return tileImg
        }
        anchors.fill: root
        anchors.margins: 2
        asynchronous: true
        clip: true
    }

    Text {
        id: txt
        text: tileText
        anchors.centerIn: root
        color: tileTextColor
        font {
            pointSize: {
                var ps = Math.min(root.width, root.height) / 3
                return ps > 1 ? ps : 1
            }

            family: "Ubuntu"
            bold: true
        }
    }

    Custom.RoundBusyIndicator {
        id: loadingImg
        anchors.fill: root
        // changing scale according to zoom of Tile
        scale: 2 * Math.min(root.width, root.height)/300
    }
}
