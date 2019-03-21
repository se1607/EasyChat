import QtQuick 2.4
import QtQuick.Controls 1.2

import VPlayApps 1.0

ListPage {

    id:page

    title: qsTr("Dynamic")

    property alias list:l
    rightBarItem: IconButtonBarItem{
        icon:IconType.plus
        onClicked: InputDialog.inputTextMultiLine(app,qsTr("New Dynamic"),qsTr("Enter text..."),function(ok,text){
            if(ok)
                addDynamic(text)}
            )
    }
    titleItem: Icon {
      icon: IconType.star
      color: "white"
      size: dp(24)
    }

    ListModel{
        id:l
        ListElement{
            picture:"dd"
            name:"tom"
            time:"8:00"
            message:"mmmmmmmmmmm"
        }
        ListElement{
            picture:"ddf"
            name:"mony"
            time:"5:00"
            message:"mmmmgggggggggmmmmmmm"
        }
    }

    AppListView{
        model: l
        delegate: DynamicRow{}
    }

//    Timer{
//        id: loadNewTimer
//        interval: 5000
//        onTriggered: {

//          //DataModel.addTweet("Lorem Ipsum.")
//        }
//    }

    function addDynamic(text) {
        l.append({picture:"dd",name:"ff",time:"9:00",message:text})
    }

}
