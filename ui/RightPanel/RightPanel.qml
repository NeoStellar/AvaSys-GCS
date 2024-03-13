import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import QtQuick.Shapes 1.14
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
        //mapboxgl osm
        name: "osm"


        /*
        PluginParameter{
            name:"mapboxgl.access_token"
            value:"pk.eyJ1Ijoid2h5c2hpcm8iLCJhIjoiY2xzN2RhYjlrMjJ6MjJtcndlcDd5ZjZsdCJ9.53kyyo_B9LgZ5luCpkhayA"
        }
        PluginParameter{
            name:"mapboxgl.mapping.additional_style_urls"
            value:"https://api.maptiler.com/maps/satellite/tiles.json?key=B89VCP4PH8mLmIWPxmBH"
        } */

    }

    Map{
        id: map
        plugin: mapPlugin
        center: QtPositioning.coordinate(37.53, -122.31)
        zoomLevel: 12
        activeMapType: supportedMapTypes[supportedMapTypes.length-7]
        anchors.fill: parent
        MapItemView{
            id: l
            model: planeController.planes
            delegate: MapQuickItem {
                property var plane: modelData;
                function getSelection(){
                    for(var i= 0; i < planeController.planes.length; ++i){
                        var plane = planeController.planes[i];
                        if(plane.isSelected){
                            return plane.sysid;
                        }
                    }
                }
                function printPlaneInfo() {
                    console.log("Plane " + " info:");
                    console.log("Sysid: " + plane.sysid);
                    console.log("Latitude: " + plane.latitude);
                    console.log("Longitude: " + plane.longitude);
                    console.log("Altitude: " + plane.altitude);
                    console.log("Yaw: " + plane.yaw);
                    console.log("Selected: " + plane.isSelected);
                }
                sourceItem: Item{
                    id: plane2
                    transform: Rotation {
                        id: markerRotation
                        origin.x: plane2.width / 2
                        origin.y: plane2.height / 2
                        angle: plane.ihaYonelme;
                    }
                    Image {
                        id: image;
                        source: (plane.isSelected) ? "qrc:/ui/assets/plane_selected.png" : "qrc:/ui/assets/plane.png";
                        anchors.centerIn: parent
                        width: 50
                        height: 50
                        MouseArea {
                            width: parent.width
                            height: parent.height
                            onClicked: {
                                //console.log(planeController.planes[index].isSelected)
                                //planeController.changeSelection((getPlane().sysid === -1) ? getPlane().teamid : getPlane().sysid);
                                planeController.changeSelection(plane.sysid, plane.teamid);
                                popupPanel1.visible = plane.isSelected;
                            }
                        }
                        //NumberAnimation on rotation { from: 0; to: 360; duration: 2000; loops: Animation.Infinite; }
                    }

                    Rectangle {
                        width: 2
                        height: 100
                        color: "black"
                        x: image.x + image.width / 2
                        y: image.y - 32 + image.height
                        transform: Rotation {
                            origin.x: width / 2
                            origin.y: height
                            angle: 180
                        }
                    }

                    width: plane2.width * 0.5
                    height: plane2.height * 0.5
                    Text {
                        text: plane.teamid === -1 ? plane.sysid : plane.teamid
                        anchors.centerIn: parent
                        font.bold: true; color: "red"
                    }

                }
                coordinate: {
                    try {
                        return QtPositioning.coordinate(plane.latitude, plane.longitude);
                    } catch (error) {
                        //console.error("Error occurred while accessing latitude and longitude properties:", error);
                        // Return a default coordinate or handle the error gracefully
                        return QtPositioning.coordinate(0, 0); // Return a default coordinate
                    }
                }
            }
        }
    }
}
