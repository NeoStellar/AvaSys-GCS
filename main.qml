import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4
import CustomTypes 1.0
import "ui/RightPanel"
import "ui/PopupPanel"
import "ui/TopPanel"
import "ui/BottomPanel"
import "ui/VideoPanel"
import "ui/DrawerPanel"

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

    TopPanel {
        id: topBar
    }

    BottomPanel {
        id: bottomPanel1
    }

    DrawerPanel {
        id: drawer
    }
}
