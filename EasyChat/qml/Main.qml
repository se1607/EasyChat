import VPlayApps 1.0
import QtQuick 2.0
import EasyChat 1.0

App {
    id:app

    property var dynmaics: []

    Client {
        id:client
    }

    LoginMainItem{
        id:lmi
    }
}
