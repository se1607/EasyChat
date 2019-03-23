import QtQuick 2.0
import VPlayApps 1.0

Item {
    anchors.fill: parent

    property bool userLoggedIn: false
    property bool userRegisterIn: false

    property var dynamiclist:[]
    property var likelist:[]

    Connections {
        target: client
        onDbDyn:{
            dynamiclist = client.getdbdynamic()
            likelist = client.getdblike()
//            console.log(dynamiclist[0])
        }
    }

    LoginPage{
        id:loginpage
        z:1
        visible: opacity > 0
        enabled: visible
        opacity: userLoggedIn ? 0 : 1
        onLoginSucceeded:{
            userLoggedIn = true
        }
        onRegistering: {
            userRegisterIn = true
            userLoggedIn = true
        }
        Behavior on opacity { NumberAnimation { duration: 250 } }
    }

    RegisterPage{
        id:registerpage
        z:1
        visible: opacity > 0
        enabled: visible
        opacity: userRegisterIn ? 1 : 0
        onReturnToLogin:{
            userRegisterIn = false
            userLoggedIn = false
        }
        Behavior on opacity { NumberAnimation { duration: 250 } }
    }

    // navigation with actual app pages
       Navigation {
         id: nav
         visible: userLoggedIn
         //enabled: userLoggedIn
         NavigationItem {
                   title: "Messaging"
                   icon: IconType.comment

                   NavigationStack {
                       //splitView: tablet
                       ConversationListPage {

                       }
                   }
               } // navigation item

               NavigationItem {
                   title: "LinkMan"
                   icon: IconType.user

                   NavigationStack {
                       LinkManPage{
                           id:linkpage
                       }
                   }
               } // navigation item

               NavigationItem{
                   id:dynamic
//                   property alias ns:ns
                   title:"Dynamic"
                   icon: IconType.empire

                   NavigationStack {
                       id:ns
                       DynamicPage{
                           id:dynamicpage
                       }
                   }
               }
       } // navigation
}
