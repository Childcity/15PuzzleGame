import QtQuick 2.9
import QtQuick.Controls 2.2

BusyIndicator {
    id: root

    contentItem: Item {
        implicitWidth: 64
        implicitHeight: 64

        Item {
            id: item
            x: parent.width / 2 - 32
            y: parent.height / 2 - 32
            width: 64
            height: 64
            opacity: root.running ? 1 : 0

            Behavior on opacity {
                OpacityAnimator {
                    duration: 250
                }
            }

            RotationAnimator {
                target: item
                running: root.visible && root.running
                from: 0
                to: 360
                loops: Animation.Infinite
                duration: 1250
            }

            Repeater {
                id: repeater
                model: 7

                Rectangle {
                    x: item.width / 2 - width / 2
                    y: item.height / 2 - height / 2
                    implicitWidth: 10 * (index > repeater.count/2
                                   ? index/repeater.count
                                   : 1)
                    implicitHeight: 10 * (index > repeater.count/2
                                          ? index/repeater.count
                                          : 1)
                    radius: 5
                    color: "#21be2b"
                    opacity: index > repeater.count/2
                             ? index/repeater.count
                             : 1
                    transform: [
                        Translate {
                            y: -Math.min(item.width, item.height) * 0.5 + 5
                        },
                        Rotation {
                            angle: index / repeater.count * 360
                            origin.x: 5
                            origin.y: 5
                            axis { x: 0; y: 0; z: 1 }
                        }
                    ]
                }
            }
        }
    }
}
