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
import "ui/RightPanel"
import "ui/PopupPanel"
import "ui/TopPanel"
import "ui/BottomPanel"
import "ui/VideoPanel"
import "ui/DrawerPanel"
import "ui/NotificationPanel"
import "ui/HorizonPanel"
import "ui/Indicators"
Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("AvaSYS - Ground Control System")

    RightPanel {
        id: rightPanel
    }
    VideoPanel {
        id: videoRect
    }

    PopupPanel {
        id: popupPanel1
    }

    BottomPanel {
        id: bottomPanel1
    }
    DrawerPanel {
        id: drawer
    }
    TopPanel {
        id: topBar
    }
    HorizonPanel {
        id: horizonPanel
    }
    NotificationPanel {
        id: notifyPanel
    }
}
