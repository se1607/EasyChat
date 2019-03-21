import QtQuick 2.4
import VPlayApps 1.0

ListPage {

    id:lp
    property string person


    property var newMsgs: []
    property var messCom: []

    property int numRepeats: 1

    readonly property int numLoadedItems: blindTextMsgs.length
    property var blindTextMsgs: [] //消息记录
    property var sendMess: []

    Connections{
        target: client
        onNewmessage:{
            messCom = client.getFriendMessages(person)
            newMsgs.push({text:messCom[messCom.length-1],me:false})
            blindTextMsgs.push({text:messCom[messCom.length-1],me:false})
            loadTimer.start()
        }
    }


    model: {

        blindTextMsgs = blindTextMsgs.concat(newMsgs)
        return blindTextMsgs
    }

    function returnNewMesModel()
    {

        return blindTextMsgs
    }




    Component.onCompleted: listView.positionViewAtEnd()

    listView.backgroundColor: "white"
    listView.anchors.bottomMargin: inputBox.height

    //    listView.header: VisibilityRefreshHandler {
    //        onRefresh: loadTimer.start()
    //    }

    //fake loading with timer
    Timer {
        id: loadTimer
        interval: 10
        onTriggered: {

            model = returnNewMesModel()
            listView.positionViewAtEnd()
        }
    }


    delegate: Item {
        id: bubble

        width: parent.width
        height: bg.height + dp(20)

        property bool me: modelData.me

        Rectangle {
            id: bg
            color: me ? Theme.tintColor : "#e9e9e9"
            radius: dp(10)

            x: me ? (bubble.width - width - dp(10)) : dp(10)
            y: dp(10)
            width: innerText.width + dp(20)
            height: innerText.implicitHeight + dp(20)

            Text {
                id: innerText
                x: dp(10)
                y: dp(10)
                width: Math.min(innerText.implicitWidth, bubble.parent.width * 0.75)
                wrapMode: Text.WordWrap
                text: modelData.text
                font.pixelSize: sp(16)
                color: me ? "white" : "black"
            }
        }
    }


    // horizontal separator line between input text and
    Rectangle {
        height: px(1)
        anchors.bottom: inputBox.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#cccccc"
    }

    AppText {
        id:friendname
        text:person
        visible:false
    }

    AppTextField {
        id: inputBox
        height: dp(48)
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: dp(8)
        anchors.rightMargin: dp(8)
        font.pixelSize: sp(16)
        placeholderText: "Type a message ..."
        backgroundColor: "white"
        verticalAlignment: Text.AlignVCenter

        onAccepted: {
            sendMess.push({text: inputBox.text,me: true})
            blindTextMsgs.push({text: inputBox.text,me: true})
            loadTimer.start()
            client.getSendMessage(inputBox.text,person)
            inputBox.text = ""
            listView.positionViewAtEnd()
        }
    }
}
