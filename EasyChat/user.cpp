#include "user.h"

User::User(QObject *parent):QObject(parent),m_name(""),m_password("")
{

}

//QQmlListProperty<QString> User::friends()
//{
//    return QQmlListProperty<QString>(this,m_friends);
//}

//void User::appendFriend(QString *u)
//{
//    m_friends.append(u);
//}

//void User::clearFriend(QString *u)
//{
//    m_friends.removeOne(u);
//}
