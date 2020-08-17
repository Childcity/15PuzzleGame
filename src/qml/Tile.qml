import QtQuick 2.11
import QtQuick.Window 2.11

Rectangle {
    id: root
	radius: 10
	color: "#bff"
	border {
		color: "black"
		width: 1
    }

    property string tileText: "0"

    Text {
        id: txt
        text: tileText
        anchors.centerIn: root
        font {
            pointSize: {
                var ps = Math.min(root.width, root.height) / 3
                return ps > 1 ? ps : 1
            }

            family: "Ubuntu"
            bold: true
        }
    }

    Image {
        id: img
        source: "file"
    }
}
