
import VPlayApps 1.0
import QtQuick 2.0

Page {
    id: mainPage

    property var conversationList:[]
    property var count
    property string tmpname

    property var messages: []

    Connections{
        target: client
        onAddConversations:{
            count = client.getConversationcount()
            console.log(count)
            client.getConversationName()
            conversationList = client.conversationlist
            console.log(conversationList[count-1])
            messages = client.getFriendMessages(conversationList[count-1])
            console.log(messages[1])
        }
    }

    AppListView {
        model:conversationList
        delegate: SimpleRow {
            text: modelData
            onSelected: {
                tmpname = modelData
                navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: modelData})
            }
        }
    }

    Component {
        id: detailPageComponent

        ConversationPage{
            id:lp
            //            person: tmpName
            onVisibleChanged: {
                if(lp.visible === false){
                    client.exitConversation()
                }
            }
        }

    }
    //  signal logoutClicked()

    //  property var myfriends:[]

    //  Connections {
    //      target:client
    //      onFriendlistChanged:{
    //          myfriends = client.friendlist
    //          console.log(myfriends[0])
    //      }
    //  }

    //  AppListView {
    //    model:myfriends
    //    delegate: SimpleRow {
    //      text: modelData
    ////      onSelected: navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: name})
    //    }
    //  }

    //  // component for detail page (opened when item of AppListView is selected)
    //  Component {
    //    id: detailPageComponent

    //    ConversationPage{}

    //  }// DetailPageComponent
}
