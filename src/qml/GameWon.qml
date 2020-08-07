import QtQuick 2.11
import QtQuick.Window 2.11
import GameBoardModel 1.0

Rectangle {
    id: root
    radius: 15
    //width: txt.contentWidth + 20
    //height: txt.contentHeight + 20
	border {
		width: 3
		color: "#368be7"
	}
	
    Text {
        id: txt
        width: root.width
        height: root.height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Game Won!")
        color: "#1c7c1c"
        minimumPointSize: 1
        fontSizeMode: Text.Fit
        wrapMode: Text.WordWrap
        font {
            pointSize: {
                var ps = Math.min(root.width, root.height) / 5
                return ps > 1 ? ps : 1
            }
            family: "Ubuntu"
            bold: true
        }
	}
}
