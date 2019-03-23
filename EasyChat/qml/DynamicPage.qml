import QtQuick 2.4
import QtQuick.Controls 1.2
import VPlayApps 1.0
import QtQml 2.0

ListPage {
    id:page
    title: qsTr("Dynamic")

    property var date:new Date()
    property var tmpValue
    property alias list:l

    Component.onCompleted: {
        for(var i=0,j=0;i < dynamiclist.length,j < likelist.length;i+=3,j++){
            l.insert(0,{picture:"$",name:dynamiclist[i],message:dynamiclist[i+1],time:dynamiclist[i+2],like:likelist[j]})
            console.log(dynamiclist[i+1])
        }
    }

    Connections{
        target:client
        onNewDynamic:{
            l.insert(0,{picture:"$",name:n,time:t,message:c})
            //            l.append()
        }
        //        onDbDyn:{
        //            console.log("hhhhhh1")
        //        }
    }

    rightBarItem: IconButtonBarItem{
        icon:IconType.plus
        onClicked: InputDialog.inputTextMultiLine(app,qsTr("New Dynamic"),qsTr("Enter text..."),function(ok,text){
            if(ok)
            {
                //                addDynamic(text)
                var years = date.getFullYear()
                var month = date.getMonth()+1
                var days = date.getDate()
                var hours = date.getHours()
                var minutes = date.getMinutes()
                var ndate = years+"-"+month+"-"+days+" "+hours+":"+minutes;
                console.log(ndate)
                client.writeDynamic(client.getLoginName(),text,ndate)
            }
        }
        )
    }

    titleItem: Icon {
        icon: IconType.star
        color: "white"
        size: dp(24)
    }

    ListModel{
        id:l
    }

    AppListView{
        model: l
        delegate: DynamicRow{
        }
    }
}
