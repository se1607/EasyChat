#ifndef CLIENT_H
#define CLIENT_H
#include <boost/asio.hpp>
#include <QObject>
#include <QDebug>
#include <string>

#define PORT 6666

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject *parent = 0);
    Q_INVOKABLE void getRegisterInfo(const QString &n, const QString &pw);
    Q_INVOKABLE void sendRegisterInfo();
    Q_INVOKABLE void connectServer();
    Q_INVOKABLE void getLoginInfo(const QString &n, const QString &pw);
    Q_INVOKABLE void sendLoginInfo();

    void receiveMessage();
    void processMessage(std::string message);
    //Q_INVOKABLE void print();
signals:
    void registersucceeded();
    void registerfailed();
    void registered();
    void logined();
    void loginpassworderror();
    void loginnameerror();
private:
    QString registerName;
    QString registerPassword;
    QString loginName;
    QString loginPassword;
    //bool registerStatus = false;
};

#endif // CLIENT_H