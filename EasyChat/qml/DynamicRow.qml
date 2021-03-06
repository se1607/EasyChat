import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4

import VPlayApps 1.0

Item {
    id: cell

    property int count: 0
    property bool flag:true
    property int i:0

    width: parent ? parent.width : 0
    height: Math.max(dp(48), innerGrid.height)

    Component.onCompleted: {
        for(var i=0;i < commentlist.length;i+=5){
            if(handleText.text === commentlist[i] && timeText.text === commentlist[i+2] && contentText.text === commentlist[i+1]){
                fruitModel.append({name:commentlist[i+3],content:commentlist[i+4]})
                addComment()
            }
//            console.log(commentlist[i]+"  "+commentlist[i+1]+"  "+commentlist[i+2]+"  "+commentlist[i+3]+"  "+commentlist[i+4])
        }
    }

    Connections {
        target:client
        onNewLike:{
            if(handleText.text === n && timeText.text === t && contentText.text === c){
                thetext.text = lll
            }
        }
        onNewComment:{
            if(handleText.text === sn && timeText.text === t && contentText.text === c){
                fruitModel.append({name:cn,content:comment})
                addComment()
            }
        }
    }

//    Connections {
//        target:lmi
//        onReadCommentary :{
//            console.log("~~~!!!@@@")
//        }
//    }

    Rectangle {
        //color:"green"
        //color: cellArea.pressed ? "#eee" : "#fff"
        anchors.fill: parent

        Behavior on color {
            ColorAnimation { duration: 150; easing.type: Easing.InOutQuad }
        }
    }

    //  MouseArea {
    //    id: cellArea

    //    enabled: cell.enabled
    //    anchors.fill: parent

    //    onClicked: selected(typeof index !== "undefined" ? index : 0)
    //  }

    // Main cell content inside this item
    GridLayout {
        id: innerGrid

        // Auto-break after 3 columns, so we do not have to set row & column manually
        columns: 4
        rowSpacing: dp(4)
        columnSpacing: dp(8)

        x: dp(10)
        width: parent.width - 2 * x

        // Top spacer
        Item {
            id: topSpacer
            width: parent.width
            height: dp(6)

            Layout.columnSpan: parent.columns
            Layout.fillWidth: true
        }

        Text {
            id: nameText
            elide: Text.ElideRight
            maximumLineCount: 1
            color: Theme.textColor
            font.family: Theme.normalFont.name
            font.bold: true
            font.pixelSize: dp(14)
            lineHeight: dp(16)
            lineHeightMode: Text.FixedHeight
            //text: cell.name
            text: picture
        }

        Text {
            id: handleText
            text:name
            elide: Text.ElideRight
            maximumLineCount: 1
            //color: Theme.secondaryTextColor
            font.family: Theme.normalFont.name
            font.pixelSize: dp(14)
            lineHeight: dp(16)
            lineHeightMode: Text.FixedHeight
            //text:cell.handle
            Layout.fillWidth: true
//            verticalAlignment: Text.AlignBottom
            Layout.preferredWidth: parent.width
        }

        Text {
            id: timeText
            text:time
            elide: Text.ElideRight
            maximumLineCount: 1
            color: Theme.secondaryTextColor
            font.family: Theme.normalFont.name
            font.pixelSize: dp(12)
            lineHeight: dp(16)
            lineHeightMode: Text.FixedHeight
            verticalAlignment: Text.AlignBottom
            Layout.alignment: Qt.AlignRight

            Layout.columnSpan: parent.columns
            Layout.fillWidth: true
        }

        Text {
            id: contentText
            color: Theme.textColor
            linkColor: Theme.tintColor
            font.family: Theme.normalFont.name
            font.pixelSize: dp(14)
            lineHeight: 1.15
            text: message//cell.text
            // width: 100
            wrapMode: Text.WordWrap
            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

        }
        Row {
            //visible: !actionsHidden
            id:row

            spacing: dp(4)

            Layout.columnSpan: 3
            Layout.fillWidth: true
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: replyIcon.height + dp(4)
            Layout.alignment: Qt.AlignBottom



            Icon {
                id: replyIcon
                icon: IconType.sharesquareo
                //color: inactiveColor
            }

            Item {
                width: dp(28)
                height: 1
            }

            Icon {
                icon: IconType.pencilsquareo
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        InputDialog.inputTextMultiLine(app,qsTr("New Comment"),
                        qsTr("Enter text..."),function(ok,text){
                        if(ok && text!==""){
//                            addComment()
                            client.getNewComment(handleText.text,timeText.text,contentText.text,client.getLoginName(),text)}
                        })
                    }
                }

                //color: isRetweeted ? retweetColor : inactiveColor
            }

            Item {
                width: dp(28)
                height: 1
            }

            Icon {
                id:thun
                icon: IconType.thumbsoup
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var a = "[" + client.getLoginName() + "]"
                        if(thetext.text.indexOf(a) !== -1){
//                            console.log("---no add~~~")
                        }else{
                            thetext.text += a
//                            console.log("!!!!!!!"+thetext.text)
                            client.sendmylike(handleText.text,
                                              timeText.text,contentText.text,thetext.text)
                        }
                    }
                }
            }
            }



            Row{
                id:good
                width: parent.width
                Layout.columnSpan: parent.columns
                Layout.fillWidth: true

                height: dp(15)
                spacing: 20

                Icon{
                    icon:IconType.thumbsup
                }


                Text {
                    id:thetext
                    font.family: Theme.normalFont.name
                    font.pixelSize: dp(14)
                    text:like
                }

            }


            Item{
                id:boot
                width: parent.width

                Layout.columnSpan: parent.columns
                Layout.fillWidth: true

                height: dp(10)

                ListModel {
                    id: fruitModel

                }

                Column{
                    Repeater{
                        model: fruitModel
                        Row{
                            spacing: dp(7)
                        Text{
                            id:columnte
                            text:name+":"
                            font.pixelSize: 15
                        }
                        Text{
                            text: content
                            font.pixelSize: 15
                        }
                        }
                    }
                }
            }
        }

        // Bottom cell divider
        Rectangle {
            id: divider1
            // color: "red"
            width: parent.width
            color: Theme.dividerColor
            height: px(1)
            anchors.bottom: parent.bottom
        }

        function addComment() {
            cell.height += 25
        }
    }
