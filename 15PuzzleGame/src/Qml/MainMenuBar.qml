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

    readonly property int dimension: AppSettings.dimension
    readonly property int imageProvider: AppSettings.imageProvider

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
            onClicked: fileMenu.popup(root.parent, x + root.leftPadding, 0)
        }
        ToolButton {
            id: editTool
            Layout.preferredHeight: root.height
            text: qsTr("Edit")
            onClicked: editMenu.popup(root.parent, x + root.leftPadding, 0)
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
            title: qsTr("Image Thematic");

            ColumnLayout {
                id: providerColLay
                spacing: 2
                RadioButton {
                    text: qsTr("Nature (imgur.com)")
                    Layout.preferredHeight: menuItemHeight /*+ 2 * imgProvider.padding*/
                    checked: root.imageProvider === ImageProviderType.Imgur
                    onToggled: AppSettings.imageProvider = ImageProviderType.Imgur
                }
                RadioButton {
                    text: qsTr("Erotic (pixels.com)")
                    Layout.preferredHeight: menuItemHeight /*+ 2 * imgProvider.padding*/
                    checked: root.imageProvider === ImageProviderType.Pixels
                    onToggled: AppSettings.imageProvider = ImageProviderType.Pixels
                }
            }
        }

        Menu {
            id: dimensionMenu
            padding: menuPading
            implicitWidth: dimensionColLay.implicitWidth + 2 * menuPading
            implicitHeight: contentItem.implicitHeight + 2 * menuPading
            enter: enterAnimation
            title: qsTr("Board Dimension");

            ColumnLayout {
                id: dimensionColLay
                spacing: 2
                Repeater {
                    model: 4
                    delegate: RadioButton {
                        text: qsTr((index+2) + "x" + (index+2))
                        Layout.preferredHeight: menuItemHeight
                        checked: (index+2) === root.dimension ? true : false
                        onToggled: AppSettings.dimension = (index+2)
                    }
                }
            }
        }
    }
}


