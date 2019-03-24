import QtQuick 2.0
import VPlayApps 1.0
import QtQuick.Layouts 1.1

Page{
    id:loginPage
    title: "Login"
    signal loginSucceeded
    signal registering
    backgroundColor: Qt.rgba(0,0,0,0.75)
    property var f: []

    Connections {
        target:client
        onLogined:{
            reminder.text = "Logined"
            loginSucceeded()
            reminder.visible = false
            //f = client.getFriends()
            //console.log(f[0])
        }
        onLoginnameerror:{
            console.log("kanzheli")
            reminder.text = "User doesn't exist"
            reminder.visible = true;
        }
        onLoginpassworderror:{
            console.log("kanzheli")
            reminder.text = "Password error"
            reminder.visible = true;
        }
        onLoggined:{
            console.log("kanzheli")
            reminder.text = "Account Loggined"
            reminder.visible = true;
        }
    }

    Rectangle {
        id: loginForm
        anchors.centerIn: parent
        color: "white"
        width: content.width + dp(48)
        height: content.height + dp(16)
        radius: dp(4)
    }

    GridLayout{
        id:content
        anchors.centerIn: loginForm
        columnSpacing: dp(20)
        rowSpacing: dp(10)
        columns: 2

        AppText{
            Layout.topMargin: dp(8)
            Layout.bottomMargin: dp(12)
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignHCenter
            text: "Login"
        }

        AppText{
            text: qsTr("Account")
            font.pixelSize: sp(12)
        }

        AppTextField{
            id:account
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
        }

        AppText {
            text: qsTr("Password")
            font.pixelSize: sp(12)
        }

        AppTextField {
            id: password
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
            echoMode: TextInput.Password
        }

        Column{
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.topMargin: dp(12)

            AppButton{
                text: qsTr("Login")
                flat:false
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    loginPage.forceActiveFocus()
                    console.debug("logging in...")
                    if(account.text.length != 0 && password.text.length != 0){
                        client.getLoginInfo(account.text,password.text)
                        client.sendLoginInfo()
                    }
                }
            }

            AppButton{
                text:qsTr("No account yet? Register now")
                flat:true
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    loginPage.forceActiveFocus()
                    console.log("registering...")
                    reminder.visible = false
                    registering()
                }
            }
            AppText {
                id:reminder
                text:""
                font.pixelSize: 16
                visible:false
                color:"red"
            }
        }
    }
}
