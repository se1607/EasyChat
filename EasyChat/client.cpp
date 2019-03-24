#include <iostream>
#include "client.h"
#include <thread>
#include <fstream>
#include <sys/io.h>
#include <dirent.h>

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

void Client::addFriend(QString name)
{
    bool exist = false;
    for(int i = 0;i != friendList.size();i++)
    {
        if(name == friendList[i])
            exist = true;
    }

    if(name == loginName)
    {
        emit addself();
    }
    else if(exist)
    {
        emit friendExist();
    }
    else {
        std::string message = "FINDFRIEND_" + name.toStdString() + "_" + loginName.toStdString();
        std::cout <<  message << std::endl;
        auto s = message.data();
        boost::system::error_code ec;
        sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
        if(ec){
            std::cout << boost::system::system_error(ec).what() << std::endl;
        }
        emit sentFriendRequest();
    }

}

void Client::refuseAddDriend(QString name)
{
    QStringList tmp;
   for(int i = 0;i != newFriends.size();i++)
   {
       if(name != newFriends[i])
           tmp.append(newFriends[i]);
   }
   newFriends = tmp;
   emit newfriendsChanged();
}

void Client::acceptAddFriend(QString name)
{
    std::string message = "ADDFRIEND_" + name.toStdString() + "_" + loginName.toStdString();
    std::cout <<  message << std::endl;
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
    friendList.append(name);
    emit friendlistChanged();
}

void Client::quit()
{
    std::string message = "QUIT_" + loginName.toStdString();
    std::cout <<  message << std::endl;
    auto s = message.data();
    boost::system::error_code ec;
    sock.write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
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
                conversations[i].appendRecod(n1,true,loginName.toStdString(),n2.toStdString(),loginName.toStdString());
                //                writeFile(loginName.toStdString(),n2.toStdString(),n1.toStdString());
            }
        }
        //        emit newmessage();
    }
    else{
        Conversation c;
        c.setName(n2);
        c.setMesageList(mess);
        c.appendRecod(n1,true,loginName.toStdString(),n2.toStdString(),loginName.toStdString());
        //        writeFile(loginName.toStdString(),n2.toStdString(),n1.toStdString());
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


void Client::mkdirFile(std::string userName)
{
    std::string name = userName;
    std::string command = "mkdir " + name;
    system(command.c_str());
    std::cout << "创建文件夹成功" << std::endl;
}

void Client::readConversation()
{
    std::string path = "./" + loginName.toStdString();
    std::vector<std::string> files;
    files = getFiles(path);
    if(files.empty())
        return;

    for(int i = 0; i != files.size();i++)
    {
        Conversation c;
        c.setName(QString::fromStdString(files[i]));

        //读消息
        std::cout << "读消息： " << std::endl;
        std::string fileName = path + "/" + files[i];
        std::cout << fileName << std::endl;
        std::ifstream infile(fileName.data());
        std::string mes;
        std::vector<std::string> messages; //存聊天记录的消息
        while(infile >> mes)
        {
            std::string tmp = mes;
            std::cout << mes << std::endl;
            messages.push_back(tmp);
        }

        std::vector<Message> allmess;
        for(int i = 0; i != messages.size();i+=3)
        {
            Message m;
            m.setMess(QString::fromStdString(messages[2+i]));
            if(messages[i] == loginName.toStdString())
            {
                m.setWho(true);
            }
            else {
                m.setWho(false);
            }
            allmess.push_back(m);
        }
        c.setConverRecord(allmess);
        if(conversations.empty())
        {
            conversations.push_back(c);
        }
        else{
            for(int i = 0;i != conversations.size();i++)
            {
                QString tmp1 = conversations[i].getName();
                if(files[i] != tmp1.toStdString()){
                    conversations.push_back(c);
                }
            }
        }
    }
    if(!conversations.empty()){
        //        std::cout << "buweikong" << std::endl;
        emit addConversations();
    }
}

std::vector<std::string> Client::getFiles(std::string path)
{
    std::vector<std::string> files;
    struct stat s;
    lstat(path.data() , &s );
    if( ! S_ISDIR( s.st_mode ) )
    {
        std::cout <<"dir_name is not a valid directory !"<< std::endl;
        //        return;
    }

    struct dirent * filename;    // return value for readdir()
    DIR * dir;                   // return value for opendir()
    dir = opendir(path.data());
    if( NULL == dir )
    {
        std::cout<<"Can not open dir "<< path << std::endl;
        //        return;
    }
    std::cout <<"Successfully opened the dir !" << std::endl;

    /* read all the files in the dir ~ */
    while( ( filename = readdir(dir) ) != NULL )
    {
        if( strcmp( filename->d_name , "." ) == 0 ||
                strcmp( filename->d_name , "..") == 0    )
            continue;
        std::string name(filename ->d_name);
        std::cout << name << std::endl;
        files.push_back(name);
    }
    return files;
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


QStringList Client::newfriends()
{
    return newFriends;
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
    std::cout << message << std::endl;
    //    std::cout << message << std::endl;
    std::vector<std::string> v;

    if(message.find("_")){
        splictString(message,v,"_");
    }
    if(v[0] == "FRIENDACCEPT")
    {
        //好友接受添加
        friendList.append(QString::fromStdString(v[1]));
        emit friendlistChanged();
    }
    if(v[0] == "FRIENDREQUEST"){
        for(int i = 1;i != v.size();i++)
        {
            //在线好友请求
            if(v[i] != "*NOMESSAGE"){
                newFriends.append(QString::fromStdString(v[i]));
            }
        }
        std::cout << "friendReauest " << newFriends.size() << std::endl;
    }

    if(v[0] == "LOGINSUCCEEDED"){
        std::vector<std::string> friends;
        friendList.append(QString("New Friends"));
        if(v.size() != 1){
            splictString(v[1],friends," ");
            if(!friends.empty()){
                for(auto i:friends){
                    QString t = QString::fromStdString(i);
                    friendList.append(t);
                    //user_friends.append(t);
                    //std::cout << (*t).toStdString()<< std::endl;
                }
            }
            if(v.size() > 2){
                //离线的好友请求
                if(v[2] == "FRIENDREQUEST"){
                    for(int i = 3;i != v.size();i++)
                    {
                        newFriends.append(QString::fromStdString(v[i]));
                    }
                    std::cout << "friendReauest " << newFriends.size() << std::endl;
                }
            }
            emit logined();
            emit friendlistChanged();
        }
        else {
            emit logined();
        }
        receiveMessage();
        mkdirFile(loginName.toStdString());
        readConversation();
        std::thread t(&Client::conversationWait,this);
        t.detach();
    }
    if(v[0] == "RECIEVEMESSAGEFROM"){

        mkdirFile(loginName.toStdString());
        std::cout << v[1] <<  " send " << v[2]  << std::endl;

        if(conversationExist(QString::fromStdString(v[1]))){
            for(int i = 0; i != conversations.size();i++)
            {
                //单条消息存储
                QString friendname = QString::fromStdString(v[1]);
                if(friendname == conversations[i].getName()){
                    conversations[i].appendMessage((QString::fromStdString(v[i+2])));
                    conversations[i].appendRecod((QString::fromStdString(v[i+2])),false,v[1],loginName.toStdString(),loginName.toStdString());
                    //                    writeFile(v[1],loginName.toStdString(),v[i+2]);
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
                c.appendRecod(QString::fromStdString(v[i]),false,v[1],loginName.toStdString(),loginName.toStdString());
                //                writeFile(v[1],loginName.toStdString(),v[i]);
            }
            c.setName(QString::fromStdString(v[1]));
            c.setMesageList(mess);
            conversations.push_back(c);
            emit addConversations();
        }

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

    }else if(message == "FRIENDONTEXIST"){ //好友不存在
        emit friendDontEXit();
    }else if(message == "NOMESSAGE")
    {

    }else if(message == "LOGGINED")
    {
        emit loggined();
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
