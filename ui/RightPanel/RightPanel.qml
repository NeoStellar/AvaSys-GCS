import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
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
        // 60.170448, 24.942046
        center: QtPositioning.coordinate(37.53, -122.31)
        zoomLevel: 12
        activeMapType: supportedMapTypes[supportedMapTypes.length-7]
        anchors.fill: parent
        MapItemView {
            id: a
            model: planeController.planes
            delegate: MapQuickItem {
                property real teamid: {
                    try {
                        var plane = getPlane();
                        //console.log("index: " + (index + 1));
                        return plane.teamid;
                    }catch(error){
                        console.log(error);
                        return -1; //planeController.planes[index + 1].sysid;
                    }

                }
                function getSelection(){
                    for(var i= 0; i < planeController.planes.length; ++i){
                        var plane = planeController.planes[i];
                        if(plane.isSelected){
                            return plane.sysid;
                        }
                    }
                }
                function printPlaneInfo() {
                    for (var i = 0; i < planeController.planes.length; ++i) {
                        var plane = planeController.planes[i];
                        console.log("Plane " + i + " info:");
                        console.log("Sysid: " + plane.sysid);
                        console.log("Latitude: " + plane.latitude);
                        console.log("Longitude: " + plane.longitude);
                        console.log("Altitude: " + plane.altitude);
                        console.log("Yaw: " + plane.yaw);
                        console.log("Selected: " + plane.isSelected);
                        // Add more properties as needed
                    }
                }
                function getPlane(){

                    // 0: unk 1: plane 2: plane
                    // -1, 0
                    var plane;
                    plane = planeController.planes[0];
                    /*
                    plane = planeController.planes[index + 1];
                    try {
                        console.log("yaw:" + plane.sysid);
                    }catch(error){
                        plane = planeController.planes[0];
                    }
                    return plane;
                    */
                    /*
                    if(index >= planeController.planes.length){
                        plane = planeController.planes[index - 1];
                    }
                    else if(index <= 0){
                        plane = planeController.planes[0];
                    }else {
                        plane = planeController.planes[index];
                    }*/
                    //console.log(plane.sysid);
                    return plane;
                }

                /*
                Connections {
                    //target: planeController.findPlane(teamid);
                    target: planeController
                    onPlanesChanged:{
                        var plane = planeController.findPlane(teamid);
                        console.log("plane: " + plane.teamid + " angle: " + plane.ihaYonelme);
                        //if(plane.teamid === teamid){
                            markerRotation.angle = plane.ihaYonelme;
                        //}

                    }
                } */
                // idk why it happens but my guess is changing the whole list
                // causes qt to confuse and add another plane while there is no such plane in the model.
                // so i have to suppress and redirect every single parameter with try catch.
                sourceItem: Item{
                    id: plane2
                    transform: Rotation {
                        id: markerRotation
                        origin.x: plane2.width / 2
                        origin.y: plane2.height / 2
                        angle: getPlane().ihaYonelme;
                    }
                    Image {
                        id: image;
                        source: {
                            try {
                                return (getPlane().isSelected) ? "qrc:/ui/assets/plane_selected.png" : "qrc:/ui/assets/plane.png"
                            }catch(error){
                                //console.error(a);
                                return "qrc:/ui/assets/plane.png";
                            }
                        }
                        //rotation: planeList[index].yaw
                        anchors.centerIn: parent
                        width: 50
                        height: 50
                        MouseArea {
                            width: parent.width
                            height: parent.height
                            onClicked: {
                                //console.log(planeController.planes[index].isSelected)
                                //planeController.changeSelection((getPlane().sysid === -1) ? getPlane().teamid : getPlane().sysid);
                                planeController.changeSelection(getPlane().sysid, getPlane().teamid);
                                popupPanel1.visible = getPlane().isSelected;
                                //console.log(planeController.planes[index].isSelected)
                                //printPlaneInfo();
                                //console.log(getPlane().teamid);
                            }
                        }

                        //NumberAnimation on rotation { from: 0; to: 360; duration: 2000; loops: Animation.Infinite; }
                    }

                    width: plane2.width * 0.5
                    height: plane2.height * 0.5
                    Text {
                        text: {
                            try {
                                //console.log("tid" + getPlane().teamid);
                                //console.log("sid:" + getPlane().sysid);
                                return getPlane().teamid === -1 ? getPlane().sysid : getPlane().teamid; //getPlane().teamid //getPlane().teamid === -1 ? getPlane().sysid : getPlane().teamid;
                            }catch(error){
                                return "";
                            }
                        }
                        anchors.centerIn: parent
                        //leftPadding: 50
                        //verticalAlignment: Text.AlignVCenter
                        //horizontalAlignment: Text.AlignHCenter
                        font.bold: true; color: "red"
                    }// Path to plane marker image

                }
                //anchorPoint.x: plane2.width / 2
                //anchorPoint.y: plane2.height
                coordinate: {
                    try {
                        return QtPositioning.coordinate(getPlane().latitude, getPlane().longitude);
                    } catch (error) {
                        //console.error("Error occurred while accessing latitude and longitude properties:", error);
                        // Return a default coordinate or handle the error gracefully
                        return QtPositioning.coordinate(999, 999); // Return a default coordinate
                    }
                }
                //coordinate: QtPositioning.coordinate(getPlane().latitude, getPlane().longitude);//model.latitude, model.longitude)
            }
        }

    }
}
