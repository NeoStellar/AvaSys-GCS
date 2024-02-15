import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle{
    id: rightPanel
    anchors{
        right: parent.right
        bottom: bottomBar.top
        top: parent.top
    }
    color: "orange"
    width: parent.width * 2/3

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

            height: parent.height / 16
            color: "#A6000000"

            Image{
                    id: lockIcon
                    anchors{
                        left: parent.left
                        top: parent.top
                        margins: 10
                    }

                    width: parent.width / 40
                    fillMode: Image.PreserveAspectFit
                    source: (systemHandler.carLocked ? "qrc:/ui/assets/padlock.png" : "qrc:/ui/assets/padlock-unlock.png")
                    MouseArea{
                        anchors.fill: parent
                        onClicked: systemHandler.setCarLocked( !systemHandler.carLocked )
                    }

                }

                Text{
                    id: dateTimeDisplay
                    anchors{
                        left: lockIcon.right
                        leftMargin: 40
                        bottom: lockIcon.bottom
                        bottomMargin: 2
                    }

                    font.pixelSize: 12
                    font.bold: true
                    color: "white"

                    text: systemHandler.currentTime

                }


                Text{
                    id: connection
                    objectName: "connectionStatus"
                    anchors{
                        left: dateTimeDisplay.right
                        leftMargin: 40
                        bottom: lockIcon.bottom
                        bottomMargin: 2
                    }

                    font.pixelSize: 12
                    font.bold: true
                    color: "white"

                    text: "Connection Status: xxxxxxx"

                }

                Button{
                    text: "Connection"
                    anchors.right: parent.right
                    onClicked: drawer.open()
                }



        }

    }


}
