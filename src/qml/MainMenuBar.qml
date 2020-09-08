import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ToolBar {
    id: root
    spacing: 2
    leftPadding: 5

    height: 30
    readonly property int menuItemHeight: 40
    readonly property int menuWidth: 200

    property var enterAnimation: Transition {
        PropertyAnimation {
            property: "opacity";
            from: 0.5; to: 1.0
            duration: 200
         }
    }

    RowLayout {
        ToolButton {
            Layout.preferredHeight: root.height
            text: qsTr("File")
            onClicked: fileMenu.popup(root.parent, x, 0)
        }
        ToolButton {
            id: editTool
            Layout.preferredHeight: root.height
            text: qsTr("Edit")
            onClicked: editMenu.popup(root.parent, x, 0)
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Menu {
        id: fileMenu
        padding: 5
        implicitWidth: menuWidth
        implicitHeight: contentItem.implicitHeight + 2 * fileMenu.padding
        enter: enterAnimation

        MenuItem {
            implicitHeight: menuItemHeight
            text: qsTr("Quit");
            onTriggered: Qt.quit()
        }
    }

    Menu {
        id: editMenu
        implicitWidth: menuWidth
        enter: enterAnimation
        padding: 5

        //implicitHeight: menuItemHeight + 2 * imgProvider.padding

        Menu {
            id: imgProvider
            padding: 5
            implicitWidth: radButLay.implicitWidth + 2 * imgProvider.padding
            implicitHeight: contentItem.implicitHeight + 2 * imgProvider.padding
            enter: enterAnimation
            title: qsTr("Image provider");

            ColumnLayout {
                id: radButLay
                spacing: 2
                RadioButton {
                    text: qsTr("www.flickr.com")
                    checked: true
                    Layout.preferredHeight: menuItemHeight /*+ 2 * imgProvider.padding*/
                }
                RadioButton {
                    text: qsTr("www.p***hub.com")
                    Layout.preferredHeight: menuItemHeight /*+ 2 * imgProvider.padding*/
                }
            }
        }
    }
}


