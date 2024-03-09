import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
Rectangle{
    id: topBar
    anchors{
        top:parent.top
        right: parent.right
        left: parent.left
    }

    height: parent.height / 12
    color: "#A6000000"
    property var stringList2: {
        return mavlinkProperties.ports();
    }
    Text {
        id: text2
        anchors.centerIn: parent
        color: "white"
        text: {
            if (mavlinkProperties.connected){
                return "Connected to " + (teknofestProperties.simMode ? "Teknofest simulation" : "Local network") + " via " + (mavlinkProperties.isSerial ? "Serial" : "TCP") + " connection.";
            }else {
                return "Not connected to network at the moment."
            }
        }
        font.pixelSize: 16
        font.bold: true
    }
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
                //bottom: lockIcon.bottom
                verticalCenter: text2.verticalCenter
                bottomMargin: 2
            }

            font.pixelSize: 16
            font.bold: true
            color: "white"
            Timer {
                interval: 500; running: true; repeat: true;
                onTriggered: {
                    //var plane = planeController.findSelected();
                    var plane = mavlinkHandler.findMainPlane();
                    //console.log(plane.airspeed);
                    try {
                        dateTimeDisplay.text = "GPS Clock: " + plane.gpsSaati.getHours() + ":" + plane.gpsSaati.getMinutes();
                    }catch(error){
                        dateTimeDisplay.text = "Clock can not be shown. (Not connected)";
                    }
                }
            }

        }

        Button{
            text: "Connection"
            anchors.right: parent.right
            anchors.verticalCenter: topBar.verticalCenter
            onClicked: drawer.open()
        }
}
