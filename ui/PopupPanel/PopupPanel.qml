import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Rectangle {
    id: popup
    width: 200
    height: 150
    color: "black"
    radius: 10
    opacity: 0.8

    visible: false
    anchors {
        top: topBar.bottom
        verticalCenter: rightPanel.verticalCenter
        left: rightPanel.left
        leftMargin: 10
        topMargin: 10
        bottomMargin: 10
    }
    function getSpeed(){
        var plane = getPlane();
        //console.log(plane.airspeed);
        try {
            return plane.airspeed;
        }catch(error){
            return 0;
        }
    }
    function getPressure(){
        var plane = getPlane();
        //console.log(plane.airspeed);
        try {
            return plane.pressure.toFixed(2);
        }catch(error){
            return 0;
        }
    }
    function getIhaInfo(){
        var plane = getPlane();
        try {
            //console.log(plane.ihaBoylam);
            if(plane.teamid !== -1){
                var ihaBoylam = plane.ihaBoylam.toFixed(4);
                var ihaDikilme = plane.ihaDikilme;
                var ihaEnlem = plane.ihaEnlem.toFixed(4);
                var ihaHiz = plane.ihaHiz;
                var ihaIrtifa = plane.ihaIrtifa;
                var ihaKilitlenme = plane.ihaKilitlenme;
                var ihaOtonom = plane.ihaOtonom;
                var ihaYatis = plane.ihaYatis;
                var ihaYonelme = plane.ihaYonelme;
                var ihaBatarya = plane.ihaBatarya;

                var ihaText = "Longitude: " + ihaBoylam + " °\n"
                        + "dikilme: " + ihaDikilme + "\n"
                        + "Latitude: " + ihaEnlem + " °\n"
                        + "Air Speed: " + ihaHiz + " m/s\n"
                        + "Altitude: " + ihaIrtifa + " m\n"
                        + "ihaKilitlenme: " + ihaKilitlenme + "\n"
                        + "ihaOtonom: " + ihaOtonom + "\n"
                        + "ihaYatis: " + ihaYatis + " °\n"
                        + "Heading: " + ihaYonelme + " °\n"
                        + "Battery: " + ihaBatarya + " %";
                return ihaText;
            }else {
                var longitute = plane.longitude.toFixed(4);
                var latitude = plane.latitude.toFixed(4);
                var altitude = plane.altitude.toFixed(1);
                var yaw = plane.yaw;
                var airspeed = plane.airspeed.toFixed(2);
                var battery = plane.remainingCapacity;
                var localText = "Longitude: " + longitute + " °\n"
                        + "Latitude: " + latitude + " °\n"
                        + "Air Speed: " +  airspeed + " m/s\n"
                        + "Altitude: " + altitude + " m\n"
                        + "Heading: " + yaw + " °" + "\n"
                        + "Battery: " + battery + " %";
                return localText;
            }

        }catch(error) {
            //console.log(error);
            //var longitute = plane.longitute;
            //var latitude = plane.latitude;
            //var altitude = plane.altitude;
            //var yaw = plane.yaw;
            //var airspeed = plane.airspeed;
            //var localText = "boylam: " + longitute + "\n"
            //        + "enlem: " + latitude + "\n"
            //        + "hiz: " +  airspeed + "\n"
            //        + "irtifa: " + altitude + "\n"
            //        + "yaw: " + yaw;
            //return localText;
            return ""
        }
    }

    function getID(){
        var plane = getPlane();
        try {
            if(plane.teamid === -1){
                return plane.sysid;
            }else {
                return plane.teamid;
            }
        }catch(error){
            return -1;
        }
    }
    function getPlane(){
        var plane = planeController.findSelected();
        //console.log(plane.sysid);
        return plane;
    }

    Text {
        id: systext
        color: "white"
        anchors {
            top: popup.top
            horizontalCenter: popup.horizontalCenter
            topMargin: 10
        }
        text: {
            var plane = planeController.findSelected();
            try {
                return "Plane Team/Sys ID: " + plane.sysid;
            }catch (error){
                return "Plane Unidentified.";
            }
        }
    }
    Connections {
        target: planeController
        onPlanesChanged:{
            systext.text = "Plane Team/Sys ID: " + popup.getID();
            speedgauge.value = popup.getSpeed();
            speedtext.text = "Speed: " + popup.getSpeed().toFixed(4) + " m/s";
            pressuregauge.value = popup.getPressure();
            pressuretext.text = "Pressure: " + popup.getPressure() + " hBa";
            sallakazan5gb.text = popup.getIhaInfo();
        }
    }
    /*
    Timer {
        interval: 500; running: true; repeat: true;
        onTriggered: {
            systext.text = "Plane Team/Sys ID: " + popup.getID();
            speedgauge.value = popup.getSpeed();
            speedtext.text = "Speed: " + popup.getSpeed().toFixed(4) + " m/s";
            pressuregauge.value = popup.getPressure();
            pressuretext.text = "Pressure: " + popup.getPressure() + " hBa";
            sallakazan5gb.text = popup.getIhaInfo();
        }
    }*/


    CircularGauge {
        id: speedgauge
        minimumValue: 0
        maximumValue: 20
        anchors {
            //centerIn: parent
            horizontalCenter: popup.horizontalCenter
            top: systext.bottom
            topMargin: 10

        }
        width: parent.width * 0.8
        height: width

        style: CircularGaugeStyle {
            labelStepSize: 1
            needle: Rectangle {
                y: outerRadius * 0.15
                implicitWidth: outerRadius * 0.03
                implicitHeight: outerRadius * 0.9
                antialiasing: true
                color: Qt.rgba(0.66, 0.3, 0, 1)//"white"//Qt.rgba(0.66, 0.3, 0, 1)
            }
        }
    }

    Text {
        id: speedtext
        color: "white"
        anchors {
            top: speedgauge.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 3
        }
        text: {
            "Speed: " + popup.getSpeed() + " m/s"
        }
    }

    CircularGauge {
        id: pressuregauge
        minimumValue: 800
        maximumValue: 1500
        anchors {
            //centerIn: parent
            horizontalCenter: popup.horizontalCenter
            top: speedtext.bottom
            topMargin: 10

        }
        width: parent.width * 0.8
        height: width
        style: CircularGaugeStyle {
            labelStepSize: 100
            needle: Rectangle {
                y: outerRadius * 0.15
                implicitWidth: outerRadius * 0.03
                implicitHeight: outerRadius * 0.9
                antialiasing: true
                color: Qt.rgba(0.66, 0.3, 0, 1) //"white"
            }
        }
    }
    Text {
        id: pressuretext
        color: "white"
        anchors {
            top: pressuregauge.bottom
            horizontalCenter: parent.horizontalCenter
            topMargin: 3
        }
        text: {
            "Pressure: " + popup.getPressure() + " m/s"
        }
    }
    Text {
        id: sallakazan5gb
        color: "white"
        anchors {
            top: pressuretext.bottom
            horizontalCenter: parent.horizontalCenter
            verticalCenter: Text.AlignVCenter
            topMargin: 10
            //left: popup.left
            bottom: popup.bottom
            //right: popup.right
            //leftMargin: 5
        }
        //fontSizeMode: Text.Fit
        text: popup.getIhaInfo();
    } 
}
