#include "user.h"

User::User(QObject *parent):QObject(parent),m_name(""),m_password("")
{

}

QQmlListProperty<User> User::friends()
{
    return QQmlListProperty<User>(this,m_friends);
}

void User::appendFriend(User *u)
{
    m_friends.append(u);
}

void User::clearFriend(User *u)
{
    m_friends.removeOne(u);
}
