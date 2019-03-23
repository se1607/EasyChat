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
public:
    Client(QObject *parent = 0);
    Q_INVOKABLE void getRegisterInfo(const QString &n, const QString &pw);
    Q_INVOKABLE void sendRegisterInfo();
    Q_INVOKABLE void connectServer();
    Q_INVOKABLE void getLoginInfo(const QString &n, const QString &pw);
    Q_INVOKABLE void sendLoginInfo();
    Q_INVOKABLE void getSendMessage(const QString &s, const QString &n2);
    Q_INVOKABLE void exitConversation();
    Q_INVOKABLE QVariant getConversationcount();
    Q_INVOKABLE void getConversationName();
    Q_INVOKABLE QStringList getFriendMessages(QString name);
    Q_INVOKABLE void writeDynamic(QString dname,QString dcontent,QString dtime);
    Q_INVOKABLE QString getLoginName();
    Q_INVOKABLE QStringList getdbdynamic();
    Q_INVOKABLE QStringList getdblike();
    Q_INVOKABLE void sendmylike(QString sendName, QString time, QString content, QString newlikes);
//    Q_INVOKABLE void writeLike(QString n);

    void receiveMessage();
    void conversationWait();
    void processMessage(std::string message);

    void splictString(std::string &s,std::vector<std::string> &v, const std::string &c);

    QStringList friendlist();

    QStringList conversationlist() const;
    QStringList friendmessages();
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

    void newDynamic(QString n,QString c,QString t);
    void dbDyn();
    void newLike(QString n,QString c,QString t,QString lll);
private:
    QString registerName;
    QString registerPassword;
    QString loginName;
    QString loginPassword;
    QStringList friendList;
    std::vector<Conversation> conversations;
    QStringList conversationList;
    QStringList friendMessages;
    QStringList dynamicList;
    QStringList likeList;
    std::vector<Dynamic> dynamiclist;
};

#endif // CLIENT_H
