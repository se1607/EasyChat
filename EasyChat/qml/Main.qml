import VPlayApps 1.0
import QtQuick 2.0
import EasyChat 1.0

App {
//    NavigationStack {

//        Page {
//            title: qsTr("Main Page")

//            Image {
//                source: "../assets/vplay-logo.png"
//                anchors.centerIn: parent
//            }
//        }

//    }
    id:app
    Client {
        id:client
    }

    LoginMainItem{
        id:lmi
    }
}
