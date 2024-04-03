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
        center: QtPositioning.coordinate(37.53, -122.31)
        zoomLevel: 12
        activeMapType: supportedMapTypes[supportedMapTypes.length-7]
        anchors.fill: parent


        Text {
            id: distanceText
            text: "a"
            visible: false
            font.pixelSize: 16
            font.bold: true
            color: "white"
            anchors.centerIn: line
        }
        MapPolyline {
            id: line
            visible: true
            line.width: 3
            line.color: "red"
            smooth: true


            function calculateDistance(lat1, lon1, lat2, lon2) {
                var R = 6371; // Radius of the earth in km
                var dLat = deg2rad(lat2 - lat1); // deg2rad below
                var dLon = deg2rad(lon2 - lon1);
                var a =
                    Math.sin(dLat / 2) * Math.sin(dLat / 2) +
                    Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) *
                    Math.sin(dLon / 2) * Math.sin(dLon / 2);
                var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
                var d = R * c; // Distance in km
                return d * 1000; // Convert to meters
            }

            function deg2rad(deg) {
                return deg * (Math.PI / 180)
            }
            function updateLine() {
                var mainPlane = mavlinkHandler.findMainPlane()
                var selectedPlane = planeController.findSelected()
                try {
                    var mainPlanePos = QtPositioning.coordinate(mainPlane.latitude, mainPlane.longitude)
                    var selectedPlanePos = QtPositioning.coordinate(selectedPlane.latitude, selectedPlane.longitude)
                    if(mainPlanePos && selectedPlanePos){
                        if(mainPlane.sysid === selectedPlane.sysid){
                            return;
                        }

                        visible = true;
                        //var distance = mainPlanePos.distanceTo(selectedPlanePos);
                        path = [mainPlanePos, selectedPlanePos]
                        var distance = selectedPlanePos.distanceTo(mainPlanePos)
                        //console.log(mainPlanePos);
                        //console.log(selectedPlanePos);
                        //console.log("mainplane:", mainPlane.sysid);
                        //console.log(distance.toFixed(2) + "m");
                        distanceText.visible = true;
                        distanceText.text = distance.toFixed(2) + "m";
                    }else {
                        path = [];
                        distanceText.text = "";
                    }


                }catch (error){
                    visible = false;
                    distanceText.visible = false;
                }
            }
        }
        Timer {
            interval: 100; running: true; repeat: true;
            onTriggered: {
                line.updateLine();
            }
        }

        Instantiator {
            model: mavlinkHandler.circles
            delegate: MapItemGroup {
                id: circlegroup
                property var circle: modelData;
                property real baseFontSize: 200

                MapCircle {
                    id: circ
                    center: QtPositioning.coordinate(circle.latitude, circle.longitude)
                    color: circle.empty ? "transparent" : circle.insideColor
                    radius: circle.radius
                    opacity: circle.opacity
                    border.color: circle.hasBorder ? circle.borderColor : "black"
                    border.width: circle.hasBorder ? circle.borderSize : 0
                    z: circle.id
                    Component.onCompleted: {
                        // Any initialization code if needed
                        console.log(circle.hasBorder);
                        console.log(circle.empty);
                        console.log(circle.insideColor)
                    }
                }

                Text {
                    text: circle.name
                    font.bold: true
                    color: "white"
                    //font.pixelSize: baseFontSize * (1 / map.zoomLevel)
                    visible: map.zoomLevel >= 10
                    anchors.centerIn: circ
                }

            }
            onObjectAdded: map.addMapItemGroup(object)
            onObjectRemoved: map.removeMapItemGroup(object)
        }

        MapQuickItem {
            sourceItem: MapItemGroup {
                Image {
                    id: asdkl;
                    source: "qrc:/ui/assets/marker.png"
                    width: 70
                    height: 100
                    x: -asdkl.width / 2
                    // Offset the image upwards so that its center aligns with the geographical point
                    y: -asdkl.height
                }
                Text {
                    text: "QR Location"
                    font.bold: true
                    color: "white"
                    //font.pixelSize: baseFontSize * (1 / map.zoomLevel)
                    visible: map.zoomLevel >= 10
                    x: -asdkl.width / 2
                    anchors.bottom: asdkl.top
                }
            }
            coordinate: QtPositioning.coordinate(37.55, -122.31)
        }

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
                        font.bold: true; color: mavlinkHandler.findMainPlane().sysid === plane.sysid ? "green" :  "red"
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
