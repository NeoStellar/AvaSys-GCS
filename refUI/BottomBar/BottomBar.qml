import QtQuick 2.15

Rectangle{
    id: bottomBar
    anchors{
        bottom: parent.bottom
        left: parent.left
        right: parent.right
    }

    height: parent.height / 16
    color: "black"

    Image{
        id: planeSettingsIcon
        anchors{
            left:parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }

        height: parent.height * .85
        fillMode: Image.PreserveAspectFit

        source:"qrc:/ui/assets/plane.png"

    }

}
