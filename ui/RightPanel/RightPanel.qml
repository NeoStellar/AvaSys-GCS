import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
Rectangle {
    id: rightPanel

    anchors{
        right: parent.right
        bottom: parent.bottom
        top: parent.top
        left: parent.left
    }

    Plugin{
        id: mapPlugin
        name: "mapboxgl"
        PluginParameter{
            name:"mapboxgl.access_token"
            value:"pk.eyJ1Ijoid2h5c2hpcm8iLCJhIjoiY2xzN2RhYjlrMjJ6MjJtcndlcDd5ZjZsdCJ9.53kyyo_B9LgZ5luCpkhayA"
        }
        PluginParameter{
            name:"mapboxgl.mapping.additional_style_urls"
            value:"https://api.maptiler.com/maps/satellite/tiles.json?key=B89VCP4PH8mLmIWPxmBH"
        }
    }

    Map{
        id: map
        plugin: mapPlugin
        center: QtPositioning.coordinate(60.170448, 24.942046)
        zoomLevel: 12
        activeMapType: supportedMapTypes[supportedMapTypes.length-7]
        anchors.fill: parent

        Rectangle{
            id: topBar
            anchors{
                top:parent.top
                right: parent.right
                left: parent.left
            }

            height: parent.height / 12
            color: "#A6000000"

            Image{
                    id: lockIcon
                    anchors{
                        left: parent.left
                        top: topBar.top
                        margins: 10
                        verticalCenter: topBar.verticalCenter
                    }

                    width: parent.width / 40
                    fillMode: Image.PreserveAspectFit
                    source: (mavlinkProperties.armed ? "qrc:/ui/assets/padlock.png" : "qrc:/ui/assets/padlock-unlock.png")

                }

                Text{
                    id: dateTimeDisplay
                    anchors{
                        left: lockIcon.right
                        leftMargin: 40
                        bottom: lockIcon.bottom
                        verticalCenter: topBar.verticalCenter
                        bottomMargin: 2
                    }

                    font.pixelSize: 12
                    font.bold: true
                    color: "white"

                    text: "31:31"

                }


                Text{
                    id: connection
                    objectName: "connectionStatus"
                    anchors{
                        left: dateTimeDisplay.right
                        leftMargin: 40
                        bottom: lockIcon.bottom
                        verticalCenter: topBar.verticalCenter
                        bottomMargin: 2
                    }

                    font.pixelSize: 12
                    font.bold: true
                    color: "white"

                    text: (mavlinkProperties.connected ? "Connection Status: Connected" : "Connection Status: Disconnected")

                }

                Button{
                    text: "Connection"
                    anchors.right: parent.right
                    anchors.verticalCenter: topBar.verticalCenter
                    onClicked: drawer.open()
                }
        }
    }
}
