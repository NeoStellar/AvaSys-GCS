import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.1
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4
import CustomTypes 1.0
import io.smth 1.0
import "../../Indicators"
Item {
    id: cHeadingMeter
    property var plane;
    CustomImage {
        anchors.fill: parent
        source: "qrc:/ui/assets/hi_face.svg"
        rotation: -cHeadingMeter.plane.yaw
    }
    Timer {
        interval: 2000 // 3 seconds
        running: true
        repeat: true
        onTriggered: {
            var plane = mavlinkHandler.findMainPlane();
            cHeadingMeter.plane = plane;
            if (plane !== null){
                console.log("plane found! sysid: " + cHeadingMeter.plane.sysid);
                NotificationCenter.sendNotification("Successfully found main plane!", 4);
                running = false;
            }else {
                console.log("plane null");
            }
        }
    }
    CustomImage {
        anchors.fill: parent
        source: "qrc:/ui/assets/hi_case.svg"
    }
}
