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
        }

        TextInput {
            id: ipTextInput
            objectName: "ipTextInput"
            width: tcpContent.width / 2
            anchors {
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
            }

            height: 30
            color: "white"
            text: "14560"
        }

        Button {
            id: tcpButton
            text: "Connect"
            width: tcpContent.width
            anchors {
                left: tcpContent.left
                right: tcpContent.right
                top: ipTextInput.bottom
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
                left: armButton.right
                right: portTextInput.right
                //horizontalCenter: tcpContent.horizontalCenter
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
    onOpenedChanged: {
        if (opened && usbComboBox.visible)
            usbComboBox.popup.open();
    }
}


