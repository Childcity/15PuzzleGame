import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import AppSettings 1.0
import ImageProviderType 1.0

ToolBar {
    id: root
    spacing: 2
    leftPadding: 5

    height: 30
    readonly property int menuItemHeight: 40
    readonly property int menuWidth: 200
    readonly property int menuPading: 5

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
        padding: menuPading
        implicitWidth: menuWidth
        implicitHeight: contentItem.implicitHeight + 2 * menuPading
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
        padding: menuPading

        Menu {
            id: imgProvider
            padding: menuPading
            implicitWidth: providerColLay.implicitWidth + 2 * menuPading
            implicitHeight: contentItem.implicitHeight + 2 * menuPading
            enter: enterAnimation
            title: qsTr("Image provider");

            ColumnLayout {
                id: providerColLay
                spacing: 2
                RadioButton {
                    text: qsTr("www.flickr.com")
                    Layout.preferredHeight: menuItemHeight /*+ 2 * imgProvider.padding*/
                    checked: AppSettings.imageProvider === ImageProviderType.Flickr
                    onToggled: AppSettings.imageProvider = ImageProviderType.Flickr
                }
                RadioButton {
                    text: qsTr("www.p***hub.com")
                    Layout.preferredHeight: menuItemHeight /*+ 2 * imgProvider.padding*/
                    checked: AppSettings.imageProvider === ImageProviderType.Phub
                    onToggled: AppSettings.imageProvider = ImageProviderType.Phub
                }
            }
        }

        Menu {
            id: dimensionMenu
            padding: menuPading
            implicitWidth: dimensionColLay.implicitWidth + 2 * menuPading
            implicitHeight: contentItem.implicitHeight + 2 * menuPading
            enter: enterAnimation
            title: qsTr("Board dimension");

            ColumnLayout {
                id: dimensionColLay
                spacing: 2
                Repeater {
                    model: 4
                    delegate: RadioButton {
                        text: qsTr((index+2) + "x" + (index+2))
                        Layout.preferredHeight: menuItemHeight
                        checked: (index+2) === AppSettings.dimension ? true : false
                        onToggled: AppSettings.dimension = (index+2)
                    }
                }
            }
        }
    }
}


