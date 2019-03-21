
import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4

import VPlayApps 1.0

Item {
  id: cell

  property int count: 0


  width: parent ? parent.width : 0
  // minimum of 48dp for interactable items recommended
  height: Math.max(dp(48), innerGrid.height)


//  signal selected(int index)
//  signal profileSelected(int index)


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

//    RoundedImage {
//      id: avatarImage

////      img.source: cell.icon
////      img.defaultSource: "../../assets/user_default.png"

//      Layout.preferredWidth: dp(50)
//      Layout.preferredHeight: dp(50)
//      //Layout.rowSpan: image.visible ? 4 : 3
//      Layout.alignment: Qt.AlignTop

////      MouseArea {
////        anchors.fill: parent
////        onClicked: profileSelected(index || 0)
////      }
//    }

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
      color: Theme.secondaryTextColor
      font.family: Theme.normalFont.name
      font.pixelSize: dp(12)
      lineHeight: dp(16)
      lineHeightMode: Text.FixedHeight
      //text:cell.handle
      Layout.fillWidth: true
      verticalAlignment: Text.AlignBottom
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
      //text: cell.time
      Layout.alignment: Qt.AlignRight
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


//      onLinkActivated: {
//        Qt.openUrlExternally(link)
//      }
    }

//    RoundedImage {
//      id: image

//      fillMode: Image.PreserveAspectCrop
//      //visible: cell.image.length > 0 && !img.error

//      //source: cell.image

//      Layout.columnSpan: 3
//      Layout.fillWidth: true
//      Layout.preferredWidth: contentText.width
//      Layout.preferredHeight: dp(320)
//      Layout.maximumWidth: contentText.width
//      Layout.maximumHeight: dp(160)

////      MouseArea {
////        anchors.fill: parent

////        onClicked: {
////          PictureViewer.show(app, image.source)
////        }
////      }
//    }

    // actions
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
            onClicked:{ InputDialog.inputTextMultiLine(app,qsTr("New Comment"),qsTr("Enter text..."),function(ok,text){
                if(ok)
                    addComment(text)}
                )
               // time.running = true
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
                count++
                goodLs.append({name:"jim"})
            }
        }
      }

      Text {
        text: count
        visible: count>0
        //color: isFaved ? favColor : inactiveColor
        font.pixelSize: sp(13)
      }
    }



    Item{
        id:good
        width: parent.width
        Layout.columnSpan: parent.columns
        Layout.fillWidth: true

        height: dp(15)
//        Icon{
//            icon: IconType.thumbsoup
//        }

        ListModel{
            id:goodLs
            ListElement{
                name:"job"
            }
            ListElement{
                name:"tom"
            }
        }

        Row{
            spacing: 15
            Icon{
                //icon: IconType.thumbsoup
                icon:IconType.thumbsup
            }

            Repeater{
                model:goodLs
                Rectangle{
                    width: goodtx.height + 10
                    height: dp(20)
                   // color: "red"
                    Text{
                        id:goodtx
                        text:name
                        font.pixelSize: 20
                        color: "red"
                    }
                }
            }
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
                Text{
                    id:columnte
                    text:name
                    font.pixelSize: 20
                }
            }
        }

//        ListView {
//            anchors.fill: parent

//            model: fruitModel
//            delegate: fruitDelegate
//        }
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

  function addComment(text) {
      fruitModel.append({name:text})
      cell.height += 20
//      console.log(innerGrid.height)
//      console.log(cell.height)
  }
}

