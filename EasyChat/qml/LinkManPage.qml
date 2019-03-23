import QtQuick 2.0
import VPlayApps 1.0

Page {
    id:linkpage

    title: "LinkMan"

    signal logoutClicked()

    property string tmpName

    AppListView {
        model:client.friendlist
        delegate: SimpleRow {
            text: modelData
            onSelected:{
                tmpName = modelData
                navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: modelData})
        }
    }

    Component {
        id: detailPageComponent

        ConversationPage{
            id:lp
            onVisibleChanged: {
                if(lp.visible === false){
                    client.exitConversation()
                }
            }
        }

    }
}
}
