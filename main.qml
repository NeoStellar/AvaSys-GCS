import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import CustomTypes 1.0
import "ui/RightPanel"

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("AvaSYS - Ground Control System")

    RightPanel {
        id: rightPanel
    }


    Rectangle {
        id: videoRect
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

                    var ihaText = "boylam: " + ihaBoylam + "\n"
                            + "dikilme: " + ihaDikilme + "\n"
                            + "enlem: " + ihaEnlem + "\n"
                            + "hiz: " + ihaHiz + "\n"
                            + "irtifa: " + ihaIrtifa + "\n"
                            + "ihaKilitlenme: " + ihaKilitlenme + "\n"
                            + "ihaOtonom: " + ihaOtonom + "\n"
                            + "ihaYatis: " + ihaYatis + "\n"
                            + "ihaYonelme: " + ihaYonelme + "\n";
                    return ihaText;
                }else {
                    var longitute = plane.longitude.toFixed(4);
                    var latitude = plane.latitude.toFixed(4);
                    var altitude = plane.altitude.toFixed(4);
                    var yaw = plane.yaw;
                    var airspeed = plane.airspeed.toFixed(4);
                    var localText = "boylam: " + longitute + "\n"
                            + "enlem: " + latitude + "\n"
                            + "hiz: " +  airspeed + "\n"
                            + "irtifa: " + altitude + "\n"
                            + "yaw: " + yaw;
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
                topMargin: 3

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
                topMargin: 5

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
            color: "red"
            anchors {
                top: pressuretext.bottom
                horizontalCenter: parent.horizontalCenter
                topMargin: 5
                left: popup.left
                right: popup.right
                leftMargin: 5
            }
            //fontSizeMode: Text.Fit
            text: popup.getIhaInfo();
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

    Rectangle {
        id: bottomPanel1
        anchors {
            right: rightPanel.right
            left: rightPanel.left
            bottom: rightPanel.bottom
        }
        height: rightPanel.height / 12
        color: "black"
        /*
        var longitute = plane.longitude.toFixed(4);
        var latitude = plane.latitude.toFixed(4);
        var altitude = plane.altitude.toFixed(4);
        var yaw = plane.yaw;
        var airspeed = plane.airspeed.toFixed(4);
        var localText = "boylam: " + longitute + "\n"
                + "enlem: " + latitude + "\n"
                + "hiz: " +  airspeed + "\n"
                + "irtifa: " + altitude + "\n"
                + "yaw: " + yaw + "\n"
                + "pressure: " + plane.pressure.toFixed(2) + " Bar";
          */
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
                return "irtifa: " + mavlinkHandler.findMainPlane().altitude.toFixed(2) + " m";
            }
        }
        Text {
            id: yt
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
                return "yaw: " + mavlinkHandler.findMainPlane().yaw + "°";
            }
        }
        Text {
            id: pt
            anchors {
                right: yt.left
                //bottom: bottomPanel1.bottom
                //top: bottomPanel1.top
                rightMargin: 10
                verticalCenter: bottomPanel1.verticalCenter
                //horizontalCenter: bottomPanel1.horizontalCenter
            }
            color: "white"
            text: {
                return "pressure: " + mavlinkHandler.findMainPlane().pressure.toFixed(2) + " Bar";
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
                return "air speed: " + mavlinkHandler.findMainPlane().airspeed.toFixed(2) + " m/s";
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
            /*
            Text {
                id: mainPlaneText
                //width: bottomPanelBig.width * 0.33
                anchors {
                    topMargin: 5
                    left: mainPlaneBarGauge.right
                    top: bottomPanelBig.top
                    right: bottomPanelBig.right
                    bottom: bottomPanelBig.bottom
                    //horizontalCenter: bottomPanelBig.horizontalCenter
                }
                color: "white"
                text: {
                    var plane = mavlinkHandler.findMainPlane();
                    var longitute = plane.longitude.toFixed(4);
                    var latitude = plane.latitude.toFixed(4);
                    var altitude = plane.altitude.toFixed(4);
                    var yaw = plane.yaw;
                    var airspeed = plane.airspeed.toFixed(4);
                    var localText = "boylam: " + longitute + "\n"
                            + "enlem: " + latitude + "\n"
                            + "hiz: " +  airspeed + "\n"
                            + "irtifa: " + altitude + "\n"
                            + "yaw: " + yaw + "\n"
                            + "pressure: " + plane.pressure.toFixed(2) + " Bar";
                    return localText;
                }
            } */
        }
    }


    Drawer {
        id: drawer
        width: 300
        height: parent.height
        edge: Qt.RightEdge

        ColumnLayout {
            spacing: 10

            // En üstteki ComboBox
            ComboBox {
                width: parent.width
                model: ["TCP", "Serial Com"]
                onCurrentIndexChanged: {
                    // TCP seçildiğinde
                    if (currentIndex === 0) {
                        tcpContent.visible = true;
                        tcpButton.visible = true;
                        usbComboBox.visible = false;
                        baudRateInput.visible = false;
                        serialButton.visible = false;
                    }
                    // Serial Com seçildiğinde
                    else if (currentIndex === 1) {
                        tcpButton.visible = false;
                        serialButton.visible = true;
                        tcpContent.visible = false;
                        usbComboBox.visible = true;
                        baudRateInput.visible = true;
                    }
                }
            }

            // TCP için giriş alanları
            Rectangle {
                id: tcpContent
                width: parent.width
                visible: false

                color: "#f0f0f0" // Açık gri arka plan rengi
                border.color: "black" // İnce siyah çerçeve rengi
                border.width: 1 // Çerçeve kalınlığı

                // İçerik yüksekliği hesaplanıyor
                height: columnLayout.height

                ColumnLayout {
                    id: columnLayout
                    spacing: 10

                    TextInput {
                        id: ipTextInput
                        objectName: "ipTextInput"
                        width: 200
                        height: 30
                        text: "127.0.0.1"
                    }


                    TextInput {
                        id: portTextInput
                        objectName: "portTextInput"
                        width: 200
                        height: 30
                        text: "14560"
                    }

                    Button {
                        id: tcpButton
                        text: "Connect"
                        onClicked: {
                            console.log("Connecting to TCP...")
                            drawer.close()
                            mavlinkHandler.initialize(ipTextInput.text, parseInt(portTextInput.text))
                            // TCP'ye bağlanma işlemleri burada gerçekleştirilebilir
                        }
                    }
                }
            }

            // Serial Com için cihaz listesi
            ComboBox {
                id: usbComboBox
                width: 200
                //model: usbDevices
                visible: false

                onCurrentIndexChanged: {
                    if (currentText !== "") {
                        console.log("Selected USB device:", currentText);
                    }
                }
            }

            // Baud rate için TextInput
            TextInput {
                id: baudRateInput
                width: 200
                height: 30
                visible: false
            }

            Button {
                id: serialButton
                text: "Connect"
                onClicked: {
                    console.log("Connecting to Serial Com...")
                    drawer.close()
                    // Serial Com'a bağlanma işlemleri burada gerçekleştirilebilir
                }
            }
            /* function updateUI() {
                    text.text = armButton.echo(rect.text) // calling StringHelper::echo
            } */

            Button {
                id: armButton
                text: mavlinkProperties.armed ? "Status: Armed" : "Status: Disarmed"
                onClicked: {
                    mavlinkProperties.toggleArmStatus();
                }
            }
        }

        onOpenedChanged: {
            if (opened && usbComboBox.visible)
                usbComboBox.popup.open();
        }
    }
}
