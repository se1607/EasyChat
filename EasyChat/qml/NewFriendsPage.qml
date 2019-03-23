import QtQuick 2.0
import VPlayApps 1.0
//import QtQuick.Controls 2.2

Page {

    title: "New Friends"
    id: newFriendsPage

    property var name
    property var newFriends: client.newfriends

    AppListView {
        id:list
        model: newFriends
        delegate: SimpleRow {
            text: modelData
            onSelected: {
                dialog.visible = true
                name = modelData
            }
        }
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
                text: qsTr("Do you want to add this friend?")
                font.pixelSize: sp(12)
                anchors.horizontalCenter: dialog.horizontalCenter
                anchors.verticalCenter: dialog.verticalCenter
            }

            Row{

                anchors.topMargin: dp(50)
                AppButton{
                    text: qsTr("Accept")
                    flat:true
                    anchors.verticalCenter: dialog.verticalCenter
                    anchors.leftMargin: dp(20)
                    onClicked: {
                        client.acceptAddFriend(name)
                        client.refuseAddDriend(name)
                        newFriends = client.newfriends
                        list.update()
                        dialog.visible = false
                    }
                }

                AppButton{
                    text:qsTr("Refuse")
                    flat:true
                    anchors.verticalCenter: dialog.verticalCenter
                    anchors.rightMargin: dp(20)
                    onClicked: {
                        client.refuseAddDriend(name)
                        newFriends = client.newfriends
                        list.update()
                        dialog.visible = false
                    }
                }

            }
        }
    }

}

