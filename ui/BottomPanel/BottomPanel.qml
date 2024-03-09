import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Rectangle {
    id: bottomPanel1
    anchors {
        right: rightPanel.right
        left: rightPanel.left
        bottom: rightPanel.bottom
    }
    height: rightPanel.height / 12
    color: "black"
    Text {
        id: bt
        anchors {
            left: bottomPanel1.left
            //bottom: bottomPanel1.bottom
            //top: bottomPanel1.top
            leftMargin: 10
            verticalCenter: bottomPanel1.verticalCenter
            //horizontalCenter: bottomPanel1.horizontalCenter
        }
        color: "white"
        text: {
            return "boylam: " + mavlinkHandler.findMainPlane().longitude.toFixed(4) + "°";
        }
    }
    Text {
        id: et
        anchors {
            left: bt.right
            //bottom: bottomPanel1.bottom
            //top: bottomPanel1.top
            leftMargin: 10
            verticalCenter: bottomPanel1.verticalCenter
            //horizontalCenter: bottomPanel1.horizontalCenter
        }
        color: "white"
        text: {
            return "enlem: " + mavlinkHandler.findMainPlane().latitude.toFixed(4) + "°";
        }
    }
    Text {
        id: at
        anchors {
            left: et.right
            //bottom: bottomPanel1.bottom
            //top: bottomPanel1.top
            leftMargin: 10
            verticalCenter: bottomPanel1.verticalCenter
            //horizontalCenter: bottomPanel1.horizontalCenter
        }
        color: "white"
        text: {
            return "altitude: " + mavlinkHandler.findMainPlane().altitude.toFixed(2) + " m";
        }
    }
    Text {
        id: bct
        anchors {
            right: bottomPanel1.right
            //bottom: bottomPanel1.bottom
            //top: bottomPanel1.top
            rightMargin: 10
            verticalCenter: bottomPanel1.verticalCenter
            //horizontalCenter: bottomPanel1.horizontalCenter
        }
        color: "white"
        text: {
            return "Battery Capacity: " + mavlinkHandler.findMainPlane().remainingCapacity.toFixed(1) + "%";
        }
    }
    Text {
        id: pt
        anchors {
            right: bct.left
            //bottom: bottomPanel1.bottom
            //top: bottomPanel1.top
            rightMargin: 10
            verticalCenter: bottomPanel1.verticalCenter
            //horizontalCenter: bottomPanel1.horizontalCenter
        }
        color: "white"
        text: {
            return "voltage: " + mavlinkHandler.findMainPlane().voltage.toFixed(1) + " V";
        }
    }
    Text {
        id: airt
        anchors {
            right: pt.left
            //bottom: bottomPanel1.bottom
            //top: bottomPanel1.top
            rightMargin: 10
            verticalCenter: bottomPanel1.verticalCenter
            //horizontalCenter: bottomPanel1.horizontalCenter
        }
        color: "white"
        text: {
            return "current: " + mavlinkHandler.findMainPlane().current.toFixed(1) + " A";
        }
    }

    Rectangle {
        id: bottomPanelBig
        anchors {
            bottom: bottomPanel1.bottom
            horizontalCenter: bottomPanel1.horizontalCenter
            //top: bottomPanel1.top
        }
        height: bottomPanel1.height * 2
        width: bottomPanel1.width / 3
        color: "black"
        radius: 30
        function getSpeed(){
            var plane = planeController.findSelected();
            console.log(plane.airspeed);
            return plane.airspeed;
        }

        CircularGauge {
            id: mainPlaneSpeedGauge
            minimumValue: 0
            maximumValue: 20

            Timer {
                interval: 500; running: true; repeat: true;
                onTriggered: {
                    //var plane = planeController.findSelected();
                    var plane = mavlinkHandler.findMainPlane();
                    //console.log(plane.airspeed);
                    try {
                        mainPlaneSpeedGauge.value = plane.airspeed;
                    }catch(error){
                        mainPlaneSpeedGauge.value = 0;
                    }
                }
            }
            //value: getSpeed();
            width: bottomPanelBig.width * 0.33
            anchors {
                topMargin: 5
                bottom: bottomPanelBig.bottom
                top: bottomPanelBig.top
                left: bottomPanelBig.left
            }



            style: CircularGaugeStyle {
                labelStepSize: 1
                needle: Rectangle {
                    y: outerRadius * 0.15
                    implicitWidth: outerRadius * 0.03
                    implicitHeight: outerRadius * 0.9
                    antialiasing: true
                    color: "white"//Qt.rgba(0.66, 0.3, 0, 1)
                }
            }

            Text {
                text: mainPlaneSpeedGauge.value.toFixed(1) // Show the value of the gauge, adjust precision as needed
                font.pixelSize: 20
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                property real a : mainPlaneSpeedGauge.height / 4;
                //anchors.centerIn: mainPlaneYawGauge
                anchors {
                    horizontalCenter: mainPlaneSpeedGauge.horizontalCenter
                    top: mainPlaneSpeedGauge.top
                    topMargin: a
                }
            }
        }

        CircularGauge {
            id: mainPlaneBarGauge
            minimumValue: 800
            maximumValue: 1500
            Timer {
                interval: 500; running: true; repeat: true;
                onTriggered: {
                    //var plane = planeController.findSelected();
                    var plane = mavlinkHandler.findMainPlane();
                    //console.log(plane.airspeed);
                    try {
                        mainPlaneBarGauge.value = plane.pressure;
                    }catch(error){
                        mainPlaneBarGauge.value = 0;
                    }
                }
            }
            //value: getSpeed();
            width: bottomPanelBig.width * 0.33
            anchors {
                topMargin: 5
                bottom: bottomPanelBig.bottom
                top: bottomPanelBig.top
                left: mainPlaneSpeedGauge.right
            }



            style: CircularGaugeStyle {
                labelStepSize: 100
                needle: Rectangle {
                    y: outerRadius * 0.15
                    implicitWidth: outerRadius * 0.03
                    implicitHeight: outerRadius * 0.9
                    antialiasing: true
                    color: "white"//Qt.rgba(0.66, 0.3, 0, 1)
                }
            }
            Text {
                text: mainPlaneBarGauge.value.toFixed(2) // Show the value of the gauge, adjust precision as needed
                font.pixelSize: 20
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                property real a : mainPlaneBarGauge.height / 4;
                //anchors.centerIn: mainPlaneYawGauge
                anchors {
                    horizontalCenter: mainPlaneBarGauge.horizontalCenter
                    top: mainPlaneBarGauge.top
                    topMargin: a
                }
            }
        }
        CircularGauge {
            id: mainPlaneYawGauge
            minimumValue: 0
            maximumValue: 360
            Timer {
                interval: 500; running: true; repeat: true;
                onTriggered: {
                    //var plane = planeController.findSelected();
                    var plane = mavlinkHandler.findMainPlane();
                    //console.log(plane.airspeed);
                    try {
                        mainPlaneYawGauge.value = plane.yaw;
                    }catch(error){
                        mainPlaneYawGauge.value = 0;
                    }
                }
            }
            //value: getSpeed();
            //width: bottomPanelBig.width * 0.33
            anchors {
                topMargin: 5
                bottom: bottomPanelBig.bottom
                top: bottomPanelBig.top
                left: mainPlaneBarGauge.right
                right: bottomPanelBig.right
            }
            style: CircularGaugeStyle {
                labelStepSize: 10
                //minimumValueAngle: -180
                //maximumValueAngle: 180
                needle: Rectangle {
                    y: outerRadius * 0.15
                    implicitWidth: outerRadius * 0.03
                    implicitHeight: outerRadius * 0.9
                    antialiasing: true
                    color: "white"//Qt.rgba(0.66, 0.3, 0, 1)
                }
            }
            Text {
                text: mainPlaneYawGauge.value // Show the value of the gauge, adjust precision as needed
                font.pixelSize: 20
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                property real a : mainPlaneYawGauge.height / 4;
                //anchors.centerIn: mainPlaneYawGauge
                anchors {
                    horizontalCenter: mainPlaneYawGauge.horizontalCenter
                    top: mainPlaneYawGauge.top
                    topMargin: a
                }
            }
        }
    }
}
