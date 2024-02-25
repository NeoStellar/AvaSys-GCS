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
        //mapboxgl
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
        // 60.170448, 24.942046
        center: QtPositioning.coordinate(37.53, -122.31)
        zoomLevel: 12
        activeMapType: supportedMapTypes[supportedMapTypes.length-7]
        anchors.fill: parent
         MapItemView {
            model: planeList // Bind to the list of planes in the PlaneController
            delegate: MapQuickItem {
                sourceItem: Column {
                    id: plane2
                    transform: Rotation {
                        id: markerRotation
                        origin.x: planeMarker.width / 2
                        origin.y: planeMarker.height / 2
                        angle: planeList[index].yaw * 60 // Initial angle, can be adjusted as needed
                    }
                    Image {
                        id: image;
                        source: "qrc:/ui/assets/plane.png"
                        //rotation: planeList[index].yaw
                        anchors.centerIn: parent
                        width: 50
                        height: 50

                        //NumberAnimation on rotation { from: 0; to: 360; duration: 2000; loops: Animation.Infinite; }
                    }

                    width: plane2.width * 0.5
                    height: plane2.height * 0.5
                    Text {
                        text: planeList[index].sysid; font.bold: true; color: "red"
                    }// Path to plane marker image
                }
                //anchorPoint.x: plane2.width / 2
                //anchorPoint.y: plane2.height
                coordinate: QtPositioning.coordinate(planeList[index].latitude, planeList[index].longitude);//model.latitude, model.longitude)
            }
        }
        /* MapItemView {
            model: planeList
            delegate: MapCircle {

                readonly property real scale0: (planeList[index].latitude)
                readonly property real scale1: (planeList[index].longitude)
                readonly property real yScale: getYScale(scale0, scale1)
                function getYScale(scale0, scale1) {
                    console.log("latitude: ", scale0, "longitude: ", scale1);
                    console.log("model: ", planeList[index].latitude)
                    return scale0;
                }
                center {
                    latitude: planeList[index].latitude
                    longitude: planeList[index].longitude
                }
                radius: 1000.0
                color: 'green'
                border.width: 3
                Component.onCompleted: {
                            console.log("All properties of plane:");
                            console.log("sysid:", planeList[index].sysid);
                            console.log("latitude:", planeList[index].latitude);
                            console.log("longitude:", planeList[index].longitude);
                            console.log("altitude:", planeList[index].altitude);
                            // Add more properties here as needed
                        }
            }
        } */
        ListModel {
                        id:dummyModel
                        ListElement {
                            latitude: 37.53
                            longitude: -122.31
                            Azimuth: 10.0
                            Color:"red"
                         }
                    }
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
