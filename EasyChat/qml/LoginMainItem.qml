import QtQuick 2.0
import VPlayApps 1.0

Item {
    anchors.fill: parent

    property bool userLoggedIn: false
    property bool userRegisterIn: false

    LoginPage{
        id:loginpage
        z:1
        visible: opacity > 0
        enabled: visible
        opacity: userLoggedIn ? 0 : 1
        onLoginSucceeded: userLoggedIn = true
        onRegistering: {
            userRegisterIn = true
            userLoggedIn = true
        }
        Behavior on opacity { NumberAnimation { duration: 250 } }
    }

    RegisterPage{
        id:registerpage
        z:1
        visible: opacity > 0
        enabled: visible
        opacity: userRegisterIn ? 1 : 0
        onReturnToLogin:{
            userRegisterIn = false
            userLoggedIn = false
        }
        Behavior on opacity { NumberAnimation { duration: 250 } }
    }

}