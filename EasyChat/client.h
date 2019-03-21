#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <QObject>
#include <string>
#include <QQmlListProperty>
#include "conversation.h"

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
private:
    QString registerName;
    QString registerPassword;
    QString loginName;
    QString loginPassword;
    QStringList friendList;
    std::vector<Conversation> conversations;
    QStringList conversationList;
    QStringList friendMessages;
};

#endif // CLIENT_H
