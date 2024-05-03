import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Item {
    id: notify
    anchors {
        top: topBar.bottom
        horizontalCenter: bottomPanel1.horizontalCenter
    }
    width: topBar.width
    implicitWidth: element !== null ? element.implicitWidth : 0
    implicitHeight: element !== null ? element.implicitHeight : 0
    opacity: 0.8
    visible: false
    height: 200

    ListView {
        id: notificationListView
        anchors.fill: parent
        model: notificationModel
        anchors.horizontalCenter: parent.horizontalCenter
        delegate: Item {
            width: topBar.width
            property string notificationText: model.text
            property bool removePending: false
            property int category: model.category

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitWidth: element.implicitWidth
                implicitHeight: element.implicitHeight
                anchors.horizontalCenter: parent.horizontalCenter

                border.width: 1
                border.color: "black"
                color: getCategoryColor(category)
                radius: 10

                Text {
                    id: element
                    anchors.fill: parent
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    text: notificationText // Access the 'text' property of the model data
                    color: "lightgray"//"#333333"
                    font.bold: false
                    font.pixelSize: 24
                    minimumPixelSize: 10
                    fontSizeMode: Text.Fit
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.verticalCenter: parent.verticalCenter
                }

                Timer {
                    interval: 5000 // 3 seconds
                    running: true
                    repeat: false
                    onTriggered: {
                        if (!removePending)
                            notificationModel.remove(index);
                            notify.visible = false;
                    }
                }

                Component.onCompleted: {
                    notificationListView.positionViewAtEnd()
                }
            }
            function getCategoryColor(category) {
                switch (category) {
                    case 1:
                        return "lightgray";
                    case 2:
                        return "gray";
                    case 3:
                        return "lightblue";
                    case 4:
                        return "green";
                    case 5:
                        return "red";
                    case 6:
                        return "darkred";
                    default:
                        return "lightgray";
                }
            }
        }
        clip: true
        highlight: Rectangle {
            color: "transparent"
            radius: 5
            width: notificationListView.width
        }
    }

    ListModel {
        id: notificationModel
    }

    Connections {
        target: NotificationCenter
        onNewNotification: {
            notificationModel.append({text: arguments[0], category: arguments[1]});
            notify.visible = true;
        }
    }
}

