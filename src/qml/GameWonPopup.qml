import QtQuick 2.11

PopupBase {
    id: root

    background: Rectangle {
        radius: content.radius
        color: "white"
    }

    contentItem: Rectangle {
        id: content
        color: "#66c4edc4"
        radius: 15

        border {
            width: 3
            color: "#368be7"
        }

        Text {
            id: txt
            width: content.width
            height: content.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Game Won!")
            color: "#1c7c1c"

            wrapMode: Text.WordWrap
            minimumPointSize: 1
            fontSizeMode: Text.Fit
            font {
                pointSize: {
                    var ps = Math.min(content.width, content.height) / 5
                    return ps > 1 ? ps : 1
                }
                family: "Ubuntu"
                bold: true
            }
        }
    }


}

