import VPlayApps 1.0
import QtQuick 2.0

Page {

    title: "Message"
    id: conversationPage

    property var conversationList:[]  //在对话窗口显示好友名字
//    property var count
    property string tmpname

//    property var messages: []  //好友的消息

    property var allMessage: []

    rightBarItem: IconButtonBarItem{
        icon: IconType.poweroff
        onClicked: {
            dialog.visible = true

        }
    }

    Connections{
        target: client
        onAddConversations:{
//            count = client.getConversationcount()
//            console.log(count)
            client.getConversationName()
            conversationList = client.conversationlist
//            console.log(conversationList[count-1])
//            messages = client.getFriendMessages(conversationList[count-1])
//            console.log(messages[0])
        }
    }

    AppListView {
        model:conversationList
        delegate: SimpleRow {
            text: modelData
            onSelected: {
                tmpname = modelData
                allMessage = getConverRecord(tmpname)
                navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: modelData })
            }
        }
    }

    Component {
        id: detailPageComponent

        ConversationPage {
            id:lp
            newMsgs: allMessage
            person: tmpname
            onVisibleChanged: {
                if(lp.visible === false){
                    client.exitConversation()
                }
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
//            console.log("cccccccccccccccccccccccc")
//            console.log(tmp[i])
            if(tmp[1+i] === "1")
                res.push({text:tmp[i],me:true})
            else
                res.push({text:tmp[i],me:false})
        }
        return res
    }

    Rectangle{
        id:dialog
        width:200
        height: 200
        color: "white"
        visible: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Column{

            AppText {
                text: qsTr("Do you want to quit?")
                font.pixelSize: sp(12)
                anchors.horizontalCenter: dialog.parent.horizontalCenter
                anchors.verticalCenter: dialog.parent.verticalCenter
            }

            Row{

                anchors.topMargin: dp(50)
                AppButton{
                    text: qsTr("Yes")
                    flat:true
                    anchors.verticalCenter: dialog.verticalCenter
                    anchors.leftMargin: dp(20)
                    onClicked: {
                        Qt.quit()
                        dialog.visible = false
                    }
                }

                AppButton{
                    text:qsTr("No")
                    flat:true
                    anchors.verticalCenter: dialog.verticalCenter
                    anchors.rightMargin: dp(20)
                    onClicked: {

                        dialog.visible = false
                    }
                }

            }
        }
    }

}
