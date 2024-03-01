import QtQuick 2.15

Rectangle{
    id: leftPanel
    anchors{
        left: parent.left
        right: rightPanel.left
        bottom: bottomBar.top
        top: parent.top
    }

    color: "gray"

    Image{
        id: vtolRender
        anchors.centerIn: parent
        width: parent.width * 0.85

        fillMode: Image.PreserveAspectFit
        source:"qrc:/ui/assets/11.png"
    }
    Rectangle {
        id: topLBar
        anchors{
            top:parent.top
            right: parent.right
            left: parent.left
        }
        height: parent.height / 16
        color: "#A0000000"
        Image{
            id: armIcon
            anchors{
                left: parent.left
                top: parent.top
                margins: 10
            }

            width: parent.width / 20
            fillMode: Image.PreserveAspectFit
            source: (systemHandler.carLocked) ? "qrc:/ui/assets/padlock.png" : "qrc:/ui/assets/padlock-unlock.png"
            MouseArea{
                anchors.fill: parent
                onClicked: mavlinkHandler.setArmed( !mavlinkHandler.armed )
            }
        }
    }

}
