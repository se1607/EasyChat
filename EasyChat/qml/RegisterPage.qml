import QtQuick 2.0
import VPlayApps 1.0
import QtQuick.Layouts 1.1
import EasyChat 1.0

Page{
    id:registerPage
    title: "Register"
    signal returnToLogin
    backgroundColor: Qt.rgba(0,0,0,0.75)

    Connections {
        target:client
        onRegistersucceeded:{
            returninfo.text = "Succeeded!"
            returninfo.visible = true;
            returnToLogin()
        }
        onRegisterfailed:{
            returninfo.text = "Failed!"
            returninfo.visible = true;
        }
        onRegistered:{
            returninfo.text = "Registered!"
            returninfo.visible = true;
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
            text: "Register"
        }

        AppText{
            text: qsTr("Name")
            font.pixelSize: sp(12)
        }

        AppTextField{
            id: name
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
        }

        AppText{
            text: qsTr("PassWord")
            font.pixelSize: sp(12)
        }

        AppTextField{
            id: password
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
            echoMode: TextInput.Password
        }

        AppText {
            text: qsTr("Verify")
            font.pixelSize: sp(12)
        }

        AppTextField {
            id: verify
            Layout.preferredWidth: dp(200)
            showClearButton: true
            font.pixelSize: sp(14)
            borderColor: Theme.tintColor
            borderWidth: !Theme.isAndroid ? dp(2) : 0
            echoMode: TextInput.Password
        }

        Column{
            id:column
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.topMargin: dp(12)

            AppButton{
                text: qsTr("Register")
                flat:false
                anchors.horizontalCenter: column.horizontalCenter
                onClicked: {
                    registerPage.forceActiveFocus()
                    //console.debug("registerSucceed...")
                    //console.debug(name.text,password.text)
                    if(password.text != verify.text){
                        returninfo.text = "Verify Failed!"
                        returninfo.visible = true;
                    }else{
                        client.getRegisterInfo(name.text,password.text)
                        client.sendRegisterInfo()
                    }
                }
            }
            AppButton{
                text:qsTr("Return To Login Now")
                flat:true
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    returnToLogin()
                }
            }
            AppText{
                id:returninfo
                text:""
                font.pixelSize: 16
                visible: false
                color:"red"
            }
        }
    }
}
