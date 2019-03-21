#include <iostream>
#include "client.h"
#include <thread>

boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("10.253.128.111"),PORT);
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
        if(name == conversations[i].getName()){
            return conversations[i].getMesageList();
            int i = conversations[i].getMesageList().size();
            std::cout << "c++messagecount: " << i << std::endl;
        }
    }
    //    return;
}


QStringList Client::getRedordMessage(QString name)
{
    QStringList mes;
    for(int i = 0; i != conversations.size();i++)
    {
        if(name == conversations[i].getName()){
            std::vector<Message> tmp = conversations[i].getConverRecord();
            for(int i = 0; i != tmp.size();i++)
            {
                mes.append(tmp[i].getMess());
                if(tmp[i].getWho())
                    mes.append("1");
                else {
                    mes.append("0");
                }
            }
            return mes;
        }
    }
    return mes;
}

void Client::getRecord(QString name, QString s,bool b)
{
    for(int i = 0; i != conversations.size();i++)
    {
        if(name == conversations[i].getName()){
            conversations[i].appendRecod(s,b);
            std::cout << "c++ rec: " << s.toStdString() << b << std::endl;
        }
    }
}

void Client::getSendMessage(const QString &n1, const QString &n2)
{
    std::string message = "CONVERSATION_" + loginName.toStdString() + "_" + n1.toStdString() + "_" + n2.toStdString();
    //    std::cout << "-------------" << std::endl;
    std::cout <<  message << std::endl;
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }

    QStringList mess;
    mess.append(n1);

    if(conversationExist(n2)){
        for(int i = 0; i != conversations.size();i++)
        {
            if(n2 == conversations[i].getName())
            {
                conversations[i].setMesageList(mess);
                conversations[i].appendRecod(n1,true);
            }
        }
        //        emit newmessage();
    }
    else{
        Conversation c;
        c.setName(n2);
        c.setMesageList(mess);
        c.appendRecod(n1,true);
        conversations.push_back(c);
    }
    emit addConversations();
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

//void Client::startConversation()
//{
//    std::string message = "STARTCONVERSATION";
//    auto s = message.data();
//    boost::system::error_code ec;
//    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
//    if(ec){
//        std::cout << boost::system::system_error(ec).what() << std::endl;
//    }
//}

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

void Client::processMessage(std::string message)
{
    std::cout << message << std::endl;
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
            //user_friends.append(t);
            //std::cout << (*t).toStdString()<< std::endl;
        }
        emit logined();
        emit friendlistChanged();
        receiveMessage();
        std::thread t(&Client::conversationWait,this);
        t.detach();
    }
    if(v[0] == "RECIEVEMESSAGEFROM"){

        std::cout << v[1] <<  " send " << v[2]  << std::endl;

        if(conversationExist(QString::fromStdString(v[1]))){
            for(int i = 0; i != conversations.size();i++)
            {
                //单条消息存储
                QString friendname = QString::fromStdString(v[1]);
                if(friendname == conversations[i].getName()){
                    conversations[i].appendMessage((QString::fromStdString(v[i+2])));
                    conversations[i].appendRecod((QString::fromStdString(v[i+2])),false);
                }
            }
            emit addConversations();
            emit newmessage();
        }
        else {
            Conversation c;
            QStringList mess;
            for(int i = 2; i != v.size();i++)
            {
                mess.append(QString::fromStdString(v[i]));
                c.appendRecod(QString::fromStdString(v[i]),false);
            }
            c.setName(QString::fromStdString(v[1]));
            c.setMesageList(mess);
            conversations.push_back(c);
            emit addConversations();
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

bool Client::conversationExist(QString n)
{
    for(int i = 0; i != conversations.size();i++)
    {
        if(n == conversations[i].getName())
            return true;
    }
    return false;
}
