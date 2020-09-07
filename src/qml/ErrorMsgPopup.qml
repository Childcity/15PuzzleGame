import QtQuick 2.0
import QtQuick.Layouts 1.3

PopupBase {
    id: root

    property string errMsg: "Unknown error."

    background: Rectangle {
        radius: content.radius
    }

    contentItem: Rectangle {
        id: content
        color: "#66c4edc4"
        clip: true
        radius: 15

        border {
            width: 3
            color: "#368be7"
        }

        ColumnLayout {
            id: lay
            anchors.centerIn: content
            anchors.fill: content
            anchors.margins: 10
            spacing: 5

            Rectangle {
                radius: 15
                Layout.alignment: Qt.AlignTop
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 40
                Layout.rightMargin: 40

                border {
                    width: 3
                    color: "#f42c2c"
                }

                Text {
                    id: mainTxt
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.margins: 5
                    text: qsTr("Somthing went wrong :(")
                    color: "#fb7676"

                    wrapMode: Text.WordWrap
                    minimumPointSize: 1
                    fontSizeMode: Text.Fit
                    font {
                        pointSize: {
                            var ps = Math.min(content.width, content.height) / 10
                            return ps > 1 ? ps : 1
                        }
                        family: "Ubuntu"
                        bold: true
                    }
                }
            }


            Rectangle {
                Layout.alignment: Qt.AlignBottom
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.minimumWidth: 150

                Text {
                    id: descriptionTxt
                    anchors.fill: parent
                    verticalAlignment: Text.AlignHCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.margins: 5
                    text: qsTr(errMsg)
                    color: "#000"

                    wrapMode: Text.WordWrap
                    minimumPointSize: 1
                    fontSizeMode: Text.Fit
                    font {
                        pointSize: {
                            var ps = Math.min(content.width, content.height) / 15
                            return ps > 1 ? ps : 1
                        }
                        family: "Ubuntu"
                        bold: true
                    }
                }
            }
        }
    }

}
