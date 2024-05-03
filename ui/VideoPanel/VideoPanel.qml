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
    implicitHeight: 240
    color: "black"



    VideoItem {
        id: video
        //opacity: 1
        height: 240
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
                    if(video.isRunning){
                        NotificationCenter.sendNotification("Found camera!", 4);
                    }
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
    Button {
        id: qrMissionButton
        width: video.width / 2
        height: 25
        anchors {
            left: videoRect.left
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            bottom: videoRect.top
        }
        background: Rectangle {
            border.width: qrMissionButton.activeFocus ? 2 : 1
            border.color: "black"
            radius: 5
            color: "lightgray"
        }
        text: "QR Görevi"
    }

    Button {
        id: kasjdka
        height: 25
        anchors {
            left: qrMissionButton.right
            right: videoRect.right
            leftMargin: 5
            rightMargin: 5
            bottomMargin: 5
            bottom: videoRect.top
        }
        background: Rectangle {
            border.width: kasjdka.activeFocus ? 2 : 1
            border.color: "black"
            radius: 5
            color: "lightgray"
        }
        text: "Serbest Uçuş"
    }

    Button {
        id: btn1
        width: video.width / 2
        height: 25
        anchors {
            left: videoRect.left
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            bottom: qrMissionButton.top
        }
        background: Rectangle {
            border.width: btn1.activeFocus ? 2 : 1
            border.color: "black"
            radius: 5
            color: "lightgray"
        }
        text: "En Yakın Takip"
    }

    Button {
        id: btn2
        height: 25
        anchors {
            left: btn1.right
            right: videoRect.right
            leftMargin: 5
            rightMargin: 5
            bottomMargin: 5
            bottom: qrMissionButton.top
        }
        background: Rectangle {
            border.width: btn2.activeFocus ? 2 : 1
            border.color: "black"
            radius: 5
            color: "lightgray"
        }
        text: "Kaçış Görevi"
    }

}
