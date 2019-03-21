import QtQuick 2.0
import VPlayApps 1.0

Page {
    id:linkpage

    title: "LinkMan"

    signal logoutClicked()

    property string tmpName
    property var allMessage: []

    AppListView {
        model:client.friendlist
        delegate: SimpleRow {
            text: modelData
            onSelected:{
                tmpName = modelData
                allMessage = getConverRecord(tmpName)
                navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: modelData})
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
}
