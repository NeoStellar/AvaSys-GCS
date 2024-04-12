import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Drawer {
    id: drawer
    width: parent.width * 0.2
    height: parent.height
    edge: Qt.RightEdge
    background: Rectangle {
        color: "black"
        opacity: 0.8
    }

    ComboBox {
        id: connectionTypeBox
        width: drawer.width
        model: ["TCP", "Serial Com"]
        background: Rectangle {
            border.width: connectionTypeBox.activeFocus ? 2 : 1
            border.color: "black"
            color: "lightgray"
            radius: 5
        }

        onCurrentIndexChanged: {
            // TCP seçildiğinde
            if (currentIndex === 0) {
                tcpContent.visible = true;
                tcpButton.visible = true;
                usbComboBox.visible = false;
                baudRateInput.visible = false;
                serialButton.visible = false;
                serialContent.visible = false;
                takeOffButton.visible = true
            }
            // Serial Com seçildiğinde
            else if (currentIndex === 1) {
                takeOffButton.visible = false;
                serialContent.visible = true;
                tcpButton.visible = false;
                serialButton.visible = true;
                tcpContent.visible = false;
                usbComboBox.visible = true;
                baudRateInput.visible = true;
            }
        }
    }

    Rectangle {
        id: tcpContent
        width: parent.width
        anchors {
            top: connectionTypeBox.bottom
            topMargin: 20
        }

        visible: false

        color: "#f0f0f0" // Açık gri arka plan rengi
        border.color: "black" // İnce siyah çerçeve rengi
        border.width: 1 // Çerçeve kalınlığı

        // İçerik yüksekliği hesaplanıyor
        //height: columnLayout.height


        Text {
            id: preIpTextInput
            text: "IP Address"
            color: "white"
            anchors{
                left: tcpContent.left
                leftMargin: 10
            }
        }

        TextInput {
            id: ipTextInput
            objectName: "ipTextInput"
            width: tcpContent.width / 2
            anchors {
                leftMargin: 10
                top: preIpTextInput.bottom
                left: tcpContent.left
            }
            height: 30
            color: "white"
            text: "127.0.0.1"
        }
        Text {
            id: prePortTextInput
            anchors {
                left: ipTextInput.right
            }

            text: "Port Number"
            color: "white"
        }

        TextInput {
            id: portTextInput
            objectName: "portTextInput"
            width: tcpContent.width / 2
            anchors {
                top: prePortTextInput.bottom
                right: tcpContent.right
                left: prePortTextInput.left
            }

            height: 30
            color: "white"
            text: "14550"
        }

        Button {
            id: tcpButton
            text: "Connect"
            width: tcpContent.width
            anchors {
                left: tcpContent.left
                right: tcpContent.right
                top: ipTextInput.bottom
                rightMargin: 10
                leftMargin: 10
            }
            background: Rectangle {
                border.width: tcpButton.activeFocus ? 2 : 1
                border.color: "black"
                color: "lightgray"
                radius: 5
            }
            onClicked: {
                console.log("Connecting to TCP...")
                drawer.close()
                mavlinkHandler.initialize(ipTextInput.text, parseInt(portTextInput.text))
                // TCP'ye bağlanma işlemleri burada gerçekleştirilebilir
            }
        }

        Button {
            property bool pressedWithShift: false
            id: armButton
            text: mavlinkProperties.armed ? "State: Armed" : "State: Disarmed"
            anchors {
                top: tcpButton.bottom
                topMargin: 10
                left: ipTextInput.left
                //horizontalCenter: tcpContent.horizontalCenter
            }
            background: Rectangle {
                border.width: armButton.activeFocus ? 2 : 1
                border.color: "black"
                color: "lightgray"
                radius: 5
            }
            onClicked: {
                mavlinkProperties.toggleArmStatus(pressedWithShift);
            }

            Keys.onPressed: {
                // Set pressedWithShift to true when Shift is pressed
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = true
            }

            Keys.onReleased: {
                // Set pressedWithShift back to false when Shift is released
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = false
            }
        }

        Button {
            property bool pressedWithShift: false
            id: takeOffButton
            text: mavlinkProperties.isFlying ? "RTL" : "Takeoff"//mavlinkProperties.armed ? "Status: Armed" : "Status: Disarmed"
            enabled: mavlinkProperties.armed
            anchors {
                top: tcpButton.bottom
                topMargin: 10
                rightMargin: 10
                leftMargin: 5
                left: armButton.right
                right: portTextInput.right
                //horizontalCenter: tcpContent.horizontalCenter
            }
            background: Rectangle {
                border.width: takeOffButton.activeFocus ? 2 : 1
                border.color: "black"
                color: "lightgray"
                radius: 5
            }
            onClicked: {
                mavlinkHandler.flyStateChanged(!mavlinkProperties.isFlying);
                console.log(!mavlinkProperties.isFlying);
                //console.log("takeoff (" + pressedWithShift ? forced : normal + ")")
                //mavlinkProperties.toggleArmStatus(pressedWithShift);
            }

            Keys.onPressed: {
                // Set pressedWithShift to true when Shift is pressed
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = true
            }

            Keys.onReleased: {
                // Set pressedWithShift back to false when Shift is released
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = false
            }
        }
    }
    Rectangle {
        id: serialContent
        width: parent.width
        anchors {
            top: connectionTypeBox.bottom
            topMargin: 20
        }
        visible: false


        Text {
            id: usbComboText
            color: "white"
            text: "Select USB"
            anchors {
                left: serialContent.left
            }
        }
        ComboBox {
            id: usbComboBox
            //width: 200
            model: mavlinkProperties.ports
            visible: false

            anchors {
                top: usbComboText.bottom
                left: usbComboText.left
            }

            onCurrentIndexChanged: {
                if (currentText !== "") {
                    console.log("Selected USB device:", currentText);
                }
            }
        }
        Text {
            id: baudRateText
            color: "white"
            text: "Baudrate"
            anchors {
                right: serialContent.right
                left: usbComboBox.right
            }
        }

        // Baud rate için TextInput
        TextInput {
            id: baudRateInput
            //width: 200
            height: usbComboBox.height
            visible: false
            color: "white"
            text: "57600"
            anchors {
                top: baudRateText.bottom
                left: usbComboBox.right
                horizontalCenter: TextInput.AlignHCenter
                leftMargin: 5
            }
        }

        Button {
            id: serialButton
            text: "Connect"
            width: serialContent.width
            anchors {
                left: serialContent.left
                right: serialContent.right
                top: baudRateInput.bottom
                topMargin: 10
            }

            background: Rectangle {
                border.width: serialButton.activeFocus ? 2 : 1
                border.color: "black"
                radius: 5
                color: "lightgray"
            }
            onClicked: {
                console.log("Connecting to Serial Com...")
                mavlinkHandler.connectSerial("/dev/" + usbComboBox.currentText, parseInt(baudRateInput.text));
                drawer.close()
                // Serial Com'a bağlanma işlemleri burada gerçekleştirilebilir
            }
        }


        Button {
            property bool pressedWithShift: false
            id: serialArmButton
            text: mavlinkProperties.armed ? "State: Armed" : "State: Disarmed"
            anchors {
                top: serialButton.bottom
                topMargin: 10
                left: usbComboBox.left
                //horizontalCenter: tcpContent.horizontalCenter
            }

            onClicked: {
                mavlinkProperties.toggleArmStatus(pressedWithShift);
            }

            Keys.onPressed: {
                // Set pressedWithShift to true when Shift is pressed
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = true
            }

            Keys.onReleased: {
                // Set pressedWithShift back to false when Shift is released
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = false
            }
        }

        Button {
            property bool pressedWithShift: false
            id: takeOffSerialButton
            text: "Takeoff"//mavlinkProperties.armed ? "Status: Armed" : "Status: Disarmed"
            anchors {
                top: serialButton.bottom
                topMargin: 10
                right: baudRateText.right
                left: serialArmButton.right
                //horizontalCenter: tcpContent.horizontalCenter
            }

            onClicked: {
                console.log("takeoff (" + pressedWithShift ? forced : normal + ")")
                //mavlinkProperties.toggleArmStatus(pressedWithShift);
            }

            Keys.onPressed: {
                // Set pressedWithShift to true when Shift is pressed
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = true
            }

            Keys.onReleased: {
                // Set pressedWithShift back to false when Shift is released
                if (event.key === Qt.Key_Shift)
                    pressedWithShift = false
            }
        }
    }
    Rectangle {
        id: qrLocation
        color: "transparent"
        width: parent.width
        height: parent.height * 0.33
        anchors {
            bottom: teknofestContent.top
        }
        Text {
            id: qrLocationTitle
            color: "white"
            text: "QR Location"
            anchors {
                top: qrLocation.top
                left: qrLocation.left
                right: qrLocation.right
                topMargin: 5
            }
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: qrLatitude
            anchors {
                top: qrLocationTitle.bottom
                left: qrLocation.left
                leftMargin: 5
            }
            width: qrLocation.width / 2.1
            color: "white"
            text: "QR Latitude"
        }
        TextInput {
            id: qrLatitudeBox
            anchors {
                top: qrLatitude.bottom
                left: qrLatitude.left
                right: qrLocation.right
                topMargin: 10
                leftMargin: 5
            }
            color: "white"
            text: "-100.2"
        }
        Text {
            id: qrLongitude
            anchors {
                top: qrLocationTitle.bottom
                right: qrLocation.right
                topMargin: 5
            }
            width: qrLocation.width / 2.1
            color: "white"
            text: "QR Longitude"
        }
        TextInput {
            id: qrLongitudeBox
            anchors {
                top: qrLongitude.bottom
                right: qrLocation.right
                left: qrLongitude.left
                rightMargin: 5
                topMargin: 5
            }
            color: "white"
            text: "37.52"
        }
        Button {
            id: qrButton
            text: "Enable/Disable QR"
            anchors {
                top: qrLongitudeBox.bottom
                left: qrLocation.left
                right: qrLocation.right
                leftMargin: 5
                rightMargin: 5
                topMargin: 10
            }
            background: Rectangle {
                border.width: teknofestConnect.activeFocus ? 2 : 1
                border.color: "black"
                color: "lightgray"
                radius: 5
            }
        }
    }

    Rectangle {
        id: teknofestContent
        color: "transparent"
        width: parent.width
        height: parent.height * 0.33
        anchors {
            bottom: parent.bottom
        }
        Text {
            id: teknofestContentTitle
            color: "white"
            text: "Teknofest Properties"
            anchors {
                top: teknofestContent.top
                left: teknofestContent.left
                right: teknofestContent.right
                topMargin: 5
            }
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }
        TextInput {
            id: teknofestURLInput
            height: 30
            anchors {
                left: teknofestContent.left
                right: teknofestContent.right
                top: teknofestContentTitle.bottom
                topMargin: 10
                leftMargin: 5
            }
            color: "white"
            text: "http://replica.neostellar.net/api"
            horizontalAlignment: Text.AlignHCenter
        }
        Text {
            id: teknofestUserTitle
            anchors {
                top: teknofestURLInput.bottom
                left: teknofestContent.left
                leftMargin: 5
            }
            width: teknofestContent.width / 2.1
            color: "white"
            text: "Teknofest User"
        }

        TextInput {
            id: teknofestUserBox
            height: 30
            anchors {
                top: teknofestUserTitle.bottom
                left: teknofestUserTitle.left
                right: teknofestUserTitle.right
                topMargin: 5
            }
            color: "white"
            text: "kullanici2"
        }

        Text {
            id: teknofestPassTitle
            anchors {
                top: teknofestURLInput.bottom
                right: teknofestContent.right
                rightMargin: 5
            }
            width: teknofestContent.width / 2.1
            color: "white"
            text: "Teknofest Password"
        }
        TextInput{
            id: teknofestPassBox
            height: 30
            anchors {
                topMargin: 5
                top: teknofestPassTitle.bottom
                left:  teknofestPassTitle.left
                right: teknofestPassTitle.right
            }
            color: "white"
            text: "sifre"
        }

        Button {
            id: teknofestConnect
            anchors {
                top: teknofestPassBox.bottom
                right: teknofestContent.right
                left: teknofestContent.left
                leftMargin: 5
                rightMargin: 5
            }
            background: Rectangle {
                border.width: teknofestConnect.activeFocus ? 2 : 1
                border.color: "black"
                radius: 5
                color: "lightgray"
            }
            onClicked: {
                mavlinkHandler.initTeknofest(teknofestURLInput.text, teknofestUserBox.text, teknofestPassBox.text)
            }

            text: "Connect"
        }
    }

    onOpenedChanged: {
        if (opened && usbComboBox.visible)
            usbComboBox.popup.open();
    }
}


