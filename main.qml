import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "ui/RightPanel"

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")

    RightPanel {
        id: rightPanel
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
