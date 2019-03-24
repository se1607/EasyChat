#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <QObject>
#include <string>
#include <QQmlListProperty>
#include "conversation.h"
#include "dynamic.h"

#define PORT 6666

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList friendlist READ friendlist NOTIFY friendlistChanged)
    Q_PROPERTY(QStringList conversationlist READ conversationlist NOTIFY conversationListChanged)
    Q_PROPERTY(QStringList friendmessages READ friendmessages NOTIFY friendmessagesChanged)
    Q_PROPERTY(QStringList newfriends READ newfriends NOTIFY newfriendsChanged)

public:
    Client(QObject *parent = 0);
    Q_INVOKABLE void getRegisterInfo(const QString &n, const QString &pw);
    Q_INVOKABLE void sendRegisterInfo();
    Q_INVOKABLE void connectServer();
    Q_INVOKABLE void getLoginInfo(const QString &n, const QString &pw);
    Q_INVOKABLE void sendLoginInfo();

    Q_INVOKABLE void getSendMessage(const QString &s, const QString &n2);  //从qml获取要发送的消息到c++
    Q_INVOKABLE void exitConversation();
    Q_INVOKABLE QVariant getConversationcount();
    Q_INVOKABLE void getConversationName();  //获取聊天的好友名
    Q_INVOKABLE QStringList getFriendMessages(QString name); //获取和好友聊天的消息记录
    Q_INVOKABLE QStringList getRedordMessage(QString name);

    Q_INVOKABLE void addFriend(QString name);
    Q_INVOKABLE void refuseAddDriend(QString name);
    Q_INVOKABLE void acceptAddFriend(QString name);
    Q_INVOKABLE void quit();

    Q_INVOKABLE void writeDynamic(QString dname,QString dcontent,QString dtime);
    Q_INVOKABLE QString getLoginName();
    Q_INVOKABLE QStringList getdbdynamic();
    Q_INVOKABLE QStringList getdblike();
    Q_INVOKABLE void sendmylike(QString sendName, QString time, QString content, QString newlikes);
    Q_INVOKABLE void getNewComment(QString sendName,QString time,QString content,QString commentName,QString commentary);


    void receiveMessage();
    void conversationWait();
    void processMessage(std::string message);

    bool conversationExist(QString n);

    void splictString(std::string &s,std::vector<std::string> &v, const std::string &c);

    void mkdirFile(std::string userName);
    void readConversation();
    std::vector<std::string> getFiles(std::string path);

    QStringList friendlist();

    QStringList conversationlist() const;
    QStringList friendmessages();

    QStringList newfriends();

    void receiveComment();
signals:
    void registersucceeded();
    void registerfailed();
    void registered();
    void logined();
    void loginpassworderror();
    void loginnameerror();
    void friendlistChanged();
    void conversationListChanged();
    void sendmessageSusseeded();

    void addConversations();
    void friendmessagesChanged();
    void newmessage();
    void friendDontEXit();
    void sentFriendRequest();
    void newfriendsChanged();
    void addself();
    void friendExist();
    void loggined();

    void newDynamic(QString n,QString c,QString t);
    void dbDyn();
    void newLike(QString n,QString c,QString t,QString lll);
    void newComment(QString sn,QString t,QString c,QString cn,QString comment);

private:
    QString registerName;
    QString registerPassword;
    QString loginName;
    QString loginPassword;
    QStringList friendList;

    std::vector<Conversation> conversations;  //c++端读到所有会话
    QStringList conversationList;   //聊天的好友名
    QStringList friendMessages;   //和好友聊天的消息记录
    QStringList newFriends;

    QStringList dynamicList;
    QStringList likeList;
    QStringList commentList;
    std::vector<Dynamic> dynamiclist;

};

#endif // CLIENT_H
