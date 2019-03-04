
import VPlayApps 1.0
import QtQuick 2.0

Page {
  id: mainPage

  signal logoutClicked()

  ListModel {
      id:ls
        ListElement { name: "Banana" }
        ListElement { name: "Apple" }
        ListElement { name: "Potato" }
      }
  AppListView {
    model: ls
    delegate: SimpleRow {
      text: name
      onSelected: navigationStack.popAllExceptFirstAndPush(detailPageComponent, {title: name})
    }
  }

  // component for detail page (opened when item of AppListView is selected)
  Component {
    id: detailPageComponent

    ConversationPage{}

  }// DetailPageComponent
}
