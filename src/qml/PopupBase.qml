import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2

Popup {
    id: root
    modal: true
    focus: true
    dim: true

    //transformOrigin: Item.Center
    x: parent ? ((parent.width - width) / 2) : 0
    y: parent ? ((parent.height - height) / 2) : 0
    width: parent ? (parent.width / 2 - margin) : 128
    height: parent ? (parent.height / 2 - margin) : 128
    padding: 0

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnReleaseOutside

    enter: Transition {
        PropertyAnimation {
           property: "opacity";
           from: 0.3; to: 1.0
           duration: 500
        }
   }

    exit: Transition {
        NumberAnimation {
           property: "opacity";
           from: 1.0; to: 0.3
           duration: 500
        }
   }

}
