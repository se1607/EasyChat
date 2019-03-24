#include <iostream>
#include "client.h"
#include <thread>
#include <QDebug>

boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("10.253.134.223"),PORT);
boost::asio::ip::tcp::socket sock(service);
boost::asio::ip::tcp::socket sock_fileTransfer(service);

Client::Client(QObject* parent):QObject(parent)
{
    connectServer();
}

void Client::connectServer()
{
    boost::system::error_code ec;
    sock.async_connect(ep,[](const boost::system::error_code &ec){
        if(ec){
            std::cout << ec.message() << std::endl;
            return;
        }
    });
    sock_fileTransfer.async_connect(ep,[](const boost::system::error_code &ec){
        if(ec){
            std::cout << ec.message() << std::endl;
            return;
        }
    });
    service.run();
}

//void Client::print()
//{
//    qDebug() << registerName << " " << registerPassword;
//}

void Client::getRegisterInfo(const QString& n,const QString& pw)
{
    registerName = n;
    registerPassword = pw;
}

void Client::getLoginInfo(const QString &n, const QString &pw)
{
    loginName = n;
    loginPassword = pw;
}

void Client::sendRegisterInfo()
{
    std::string message = "REGISTER_" + registerName.toStdString()
            + "_" + registerPassword.toStdString();
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    std::cout << message << std::endl;
    if(ec){
        std::cout << boost::system::system_error(ec).what() <<std::endl;
        return;
    }

    receiveMessage();
}

void Client::sendLoginInfo()
{
    std::string message = "LOGIN_" + loginName.toStdString()
            + "_" + loginPassword.toStdString();
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }

    receiveMessage();
}

void Client::exitConversation()
{
    std::string exitmessage = "EXITCONVERSATION";
    auto s = exitmessage.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
}

QVariant Client::getConversationcount()
{
    int i = conversations.size();
    QVariant v = QVariant::fromValue(i);
    return v;
}

void Client::getConversationName()
{
    conversationList.clear();
    for(Conversation c : conversations)
    {
        QString t = c.getName();
        conversationList.append(t);
    }
}

QStringList Client::getFriendMessages(QString name)
{
    for(int i = 0; i != conversations.size();i++)
    {
        if(name == conversations[i].getName())
            return conversations[i].getMesageList();
    }
    //    return;
}

void Client::writeDynamic(QString dname, QString dcontent, QString dtime)
{
    Dynamic d;
    d.setPushName(dname);
    d.setPushContent(dcontent);
    d.setPushTime(dtime);
    dynamiclist.push_back(d);

    std::string message = "DYNAMIC_" + dname.toStdString() + "_" + dcontent.toStdString() + "_" + dtime.toStdString();
    std::cout << message << std::endl;
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
    emit newDynamic(dname,dcontent,dtime);
}

QString Client::getLoginName()
{
    return loginName;
}

QStringList Client::getdbdynamic()
{
    return dynamicList;
}

QStringList Client::getdblike()
{
    return likeList;
}

void Client::sendmylike(QString sendName,QString time,QString content,QString newlikes)
{
    std::string mes = "NEWLIKE_" + sendName.toStdString() + "_" + time.toStdString() + "_" + content.toStdString() + "_" + newlikes.toStdString();
    std::cout << mes << std::endl;
    auto m = mes.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(m,strlen(m)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
    //    emit newLike(newlikes);
}

void Client::getNewComment(QString sendName, QString time, QString content, QString commentName, QString commentary)
{
    std::string mes = "NEWCOMMENT_" + sendName.toStdString() + "_" + time.toStdString() + "_" + content.toStdString() + "_" + commentName.toStdString() + "_" + commentary.toStdString();
//    std::cout << mes << std::endl;
    auto m = mes.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(m,strlen(m)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
}

void Client::getSendMessage(const QString &n1, const QString &n2)
{
    std::string message = "CONVERSATION_" + loginName.toStdString() + "_" + n1.toStdString() + "_" + n2.toStdString();
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
}

void Client::receiveMessage()
{
    boost::system::error_code ec;

    try {
        char data[512];
        memset(data,0,sizeof(char) * 512);
        size_t len = sock.read_some(boost::asio::buffer(data));

        std::string s = data;
        std::cout << "Message from server:"
                  << s << std::endl;

        if(strlen(data) != 0){
            processMessage(s);
        }else{
            std::cout << "false";
        }
    }catch (boost::system::system_error e)
    {
        std::cout << e.what() << std::endl;
    }
}

void Client::conversationWait()
{
    boost::system::error_code ec;
    while(1){
        try{
            char data[512];
            memset(data,0,sizeof(char)*512);
            size_t len = sock.read_some(boost::asio::buffer(data));

            std::string s = data;
            std::cout << "Conversation from" << s << std::endl;
            if(strlen(data) != 0){
                processMessage(s);
            }else{
                std::cout << "false";
            }
        }catch (boost::system::system_error e){
            break;
        }
    }
}

void Client::splictString(std::string &s, std::vector<std::string> &v, const std::string &c)
{
    std::string::size_type pos1,pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2){
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

QStringList Client::friendlist()
{
    return friendList;
}

QStringList Client::conversationlist() const
{
    return conversationList;
}

QStringList Client::friendmessages()
{
    return friendMessages;
}

void Client::receiveComment()
{
    std::string message = "GETCOMMENTSIGNAL_";
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
    receiveMessage();
}

void Client::processMessage(std::string message)
{
    //    std::cout << message << std::endl;
    std::vector<std::string> v;

    if(message.find("_")){
        splictString(message,v,"_");
    }
    if(v[0] == "LOGINSUCCEEDED"){
        std::vector<std::string> friends;
        splictString(v[1],friends," ");
        for(auto i:friends){
            QString t = QString::fromStdString(i);
            friendList.append(t);
        }
        emit logined();
        emit friendlistChanged();
        receiveMessage();
        std::thread t(&Client::conversationWait,this);
        t.detach();
    }
    if(v[0] == "RECIEVEMESSAGEFROM"){

        std::cout << v[1] <<  " send " << v[2]  << std::endl;
        Conversation c;
        c.setName(QString::fromStdString(v[1]));
        QStringList mess;
        for(int i = 2; i != v.size();i++)
        {
            mess.append(QString::fromStdString(v[i]));
        }
        c.setMesageList(mess);
        conversations.push_back(c);
        emit addConversations();
    }
    if(v[0] == "DYNAMIC"){
        QString n = QString::fromStdString(v[1]);
        QString c = QString::fromStdString(v[2]);
        QString t = QString::fromStdString(v[3]);
        emit newDynamic(n,c,t);
    }
    if(v[0] == "DYNAMICFROM"){
        for(int i=1;i<v.size();i+=5){
            std::vector<std::string> likes;
            splictString(v[i+3],likes,"_");
            Dynamic d;
            d.setPushName(QString::fromStdString(v[i+0]));
            d.setPushContent(QString::fromStdString(v[i+1]));
            d.setPushTime(QString::fromStdString(v[i+2]));
            //            splictString(v[i+3],likes," ");
            d.setLike(QString::fromStdString(v[i+3]));
            dynamicList.append(QString::fromStdString(v[i+0]));
            dynamicList.append(QString::fromStdString(v[i+1]));
            dynamicList.append(QString::fromStdString(v[i+2]));
            likeList.append(QString::fromStdString(v[i+3]));
            dynamiclist.push_back(d);
        }
        std::cout << dynamiclist.size() <<std::endl;
        emit dbDyn();
        receiveComment();
    }
    if(v[0] == "UPDATELIKE"){
        QString n = QString::fromStdString(v[1]);
        QString c = QString::fromStdString(v[2]);
        QString t = QString::fromStdString(v[3]);
        if(v.size() == 5){
            QString lll = QString::fromStdString(v[4]);
            emit newLike(n,c,t,lll);
        }else if(v.size() == 9){
            QString lll = QString::fromStdString(v[8]);
            emit newLike(n,c,t,lll);
        }
    }
    if(v[0] == "NEWCOMMENTARY"){
        QString sn = QString::fromStdString(v[1]);
        QString t = QString::fromStdString(v[2]);
        QString c = QString::fromStdString(v[3]);
        QString cn = QString::fromStdString(v[4]);
        if(v.size() == 6){
            QString comment = QString::fromStdString(v[5]);
            emit newComment(sn,t,c,cn,comment);
        }else if(v.size() == 11){
            QString comment = QString::fromStdString(v[10]);
            emit newComment(sn,t,c,cn,comment);
        }
        for(int i=0;i<v.size();i++){
            std::cout << i << ":::    " << v[i] << std::endl;
        }
    }
    if(v[0] == "COMMENTFROM"){
//        for(auto &vvv:v){
//            std::cout << vvv << std::endl;
//        }
        for(int i=1;i < v.size();i+=5){
            QString sn = QString::fromStdString(v[i]);
            QString t = QString::fromStdString(v[i+1]);
            QString c = QString::fromStdString(v[i+2]);
            QString cn = QString::fromStdString(v[i+3]);
            QString commentary = QString::fromStdString(v[i+4]);
            emit newComment(sn,t,c,cn,commentary);
        }
    }

    if(message == "REGISTERSUCCEEDED"){
        emit registersucceeded();
    }
    else if(message == "REGISTERFAILED"){
        emit registerfailed();
    }
    else if(message == "REGISTERED"){
        emit registered();
    }
    else if(message == "NOACCOUNT"){
        emit loginnameerror();
    }
    else if(message == "PASSWORDERROR"){
        emit loginpassworderror();
    }
    else if(message == "SENDMESSAGE"){
        emit sendmessageSusseeded();
    }
}
