import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import CustomTypes 1.0

Rectangle {
    property bool sd: false
    id: videoRect
    visible: sd
    //opacity: 0.8
    radius: 10
    anchors {
        right: bottomPanel1.right
        bottom: bottomPanel1.top
    }
    width: 370
    height: 240
    color: "black"



    VideoItem {
        id: video
        //opacity: 1
        anchors {
            //fill: parent
            //centerIn: videoRect
            leftMargin: 5
            rightMargin: 5
            topMargin: 5
            bottomMargin: 5
            top: videoRect.top
            bottom: videoRect.bottom
            left: videoRect.left
            right: videoRect.right
        }
        Component.onCompleted: {
            video.start_gst()
        }

        Timer {
            interval: 500; running: true; repeat: true
            onTriggered: {
                if(sd !== video.isRunning){
                    console.log("Different values! Changing camera status.");
                }
                sd = video.isRunning;
            }
        }
    }
    Text {
        id: cameraText
        color: "white"
        anchors {
            top: videoRect.top
            topMargin: 5
            horizontalCenter: videoRect.horizontalCenter
        }
        text: "Camera Feed"
    }
}
