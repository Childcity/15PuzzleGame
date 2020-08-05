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
            pointSize: root.width !== 0 && root.height !== 0
                       ? Math.min(root.width, root.height) / 3
                       : 1
            family: "Ubuntu"
            bold: true
        }
    }
}
