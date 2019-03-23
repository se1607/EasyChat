import QtQuick 2.0
import VPlayApps 1.0

Page {
    id:linkpage

    title: "LinkMan"

    signal logoutClicked()

    property string tmpName
    property var allMessage: []

    //添加好友
    rightBarItem: IconButtonBarItem{
        icon:IconType.plus
        onClicked:{ InputDialog.inputTextSingleLine(app,qsTr("Add Friend"),qsTr("Friend Name"),function(ok,text){
            if(ok)
            {
                client.addFriend(text)
            }
        }
        )
        }
    }

    Connections{
        target: client
        onFriendDontEXit:{
            pointDialog.visible = true
            point.text = "The user don't exist."
            loadTimer.start()
        }
        onSentFriendRequest:{
            pointDialog.visible = true
            point.text = "Request sent!"
            loadTimer.start()
        }
        onFriendlistChanged:{
            list.update()
        }
        onAddself:{
            pointDialog.visible = true
            point.text = "You can't add yourself."
            loadTimer.start()
        }
        onFriendExist:{
            pointDialog.visible = true
            point.text = "The friend exist."
            loadTimer.start()
        }
    }

    Timer {
        id: loadTimer
        interval: 1000
        onTriggered: {
            pointDialog.visible = false
        }
    }

    //提示框
    Rectangle{
        id:pointDialog
        visible: false
        color: "#F5F5F5"
        height: 90
        width: 180
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        z:1
        radius: dp(4)
        Text {
            id: point
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("")
            font.pointSize: dp(10)
        }
//        MouseArea{
//            anchors.fill:parent
//            onClicked: {
//                pointDialog.visible = false
//            }
//        }
    }

    AppListView {
        id:list
        model:client.friendlist
        delegate: SimpleRow {
            text: modelData
            onSelected:{
                tmpName = modelData
                if(tmpName == "New Friends")
                {
                    navigationStack.popAllExceptFirstAndPush(newFriendPageComponent, {title: modelData})
                }
                else{
                    allMessage = getConverRecord(tmpName)
                    navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: modelData})
                }
            }
        }

        Component {
            id: detailPageComponent

            ConversationPage{
                id:lp
                person: tmpName
                newMsgs: allMessage
                onVisibleChanged: {
                    if(lp.visible === false){
                        client.exitConversation()
                    }
                }
            }

        }

        Component {
            id: newFriendPageComponent

            NewFriendsPage{
                id:newfriend

            }

        }
    }

    function  getConverRecord(fname)
    {
        var tmp = []
        tmp = client.getRedordMessage(fname)
        var res = []
        for(var i = 0; i != tmp.length;i+=2)
        {
            if(tmp[1+i] === "1")
                res.push({text:tmp[i],me:true})
            else
                res.push({text:tmp[i],me:false})
        }
        return res
    }

}
