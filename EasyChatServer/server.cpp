#include <iostream>
#include "server.h"
#include "user.h"
#include <string>
#include "conversation.h"

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(),PORT);
boost::asio::ip::tcp::acceptor acc(service,ep);

Server::Server()
{
    _userBroker = UserBroker::getInstance();
    _userBroker->selectAllUsers();
    //    _userBroker->getConversation();
}

void Server::acceptConnect()
{
    bool value = true;
    setsockopt(1,SOL_SOCKET,SO_REUSEADDR,(void *)&value,sizeof(value));
    while(1){
        socket_ptr sock(new boost::asio::ip::tcp::socket(service));
        acc.accept(*sock);
        boost::thread(boost::bind(&Server::receiveMessage,this,sock));
    }
}

void Server::receiveMessage(socket_ptr sock)
{
    boost::system::error_code ec;
    auto ep = sock->remote_endpoint(ec);
    if(ec)
    {
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
    //    std::cout << ep.address().to_string()
    //              << "已连接" << std::endl;
    std::string name;
    while(1){
        try {
            char datarec[512];
            memset(datarec,0,sizeof(char) * 512);
            size_t len = sock->read_some(boost::asio::buffer(datarec));

            std::cout << "Message from client:"
                      << datarec << std::endl;
            std::string s = datarec;
            if(strlen(datarec) != 0)
            {
                name = processMessage(s,sock);
                _sock[name] = sock;
            }
        }
        catch (boost::system::system_error e)
        {
            std::cout << e.what() << std::endl;
            std::cout << name << std::endl;
            _userBroker->deletLoginUser(name);
            std::cout << std::endl;
            _userBroker->printLoginUser();
            break;
        }
    }
}

std::string Server::processMessage(std::string message, socket_ptr sock)
{
    std::cout << "=====Process Message======" << std::endl;

    std::vector<std::string> mes;
    if(message.find("_"))
    {
        splictString(message,mes,"_");
    }
    if(mes[0] == "REGISTER")
    {
        userRegister(mes[1],mes[2],sock);
    }
    else if(mes[0] == "LOGIN")
    {
        userLogin(mes[1],mes[2],sock);
        return  mes[1];
    }
    else if(mes[0] == "CONVERSATION") {
        std::cout << message << std::endl;
        conversation(mes[1],mes[2],mes[3],sock);
        return  mes[1];
    }else if(mes[0] == "EXITCONVERSATION"){
        std::cout << "exit" << std::endl;
        return "EXIT";
    }else if(mes[0] == "FINDFRIEND"){
        addfriend(mes[1],mes[2]);
    }else if(mes[0] == "ADDFRIEND"){
        acceptFriend(mes[1],mes[2]);
    }else if (mes[0] == "QUIT") {
        _userBroker->deletLoginUser(mes[1]);
        _userBroker->printLoginUser();
    }else if(mes[0] == "DYNAMIC"){
        std::cout << mes[1] << mes[2] << mes[3] << std::endl;
        _userBroker->addDynamic(mes[1],mes[2],mes[3]);
        tranfserDynamic(mes[1],mes[2],mes[3]);
    }else if(mes[0] == "NEWLIKE"){
        std::cout << "?????:" << mes[1] << mes[4] << mes.size() << std::endl;
        tranfserLike(mes[1],mes[3],mes[2],mes[4]);
        _userBroker->updateDynamic(mes[1],mes[3],mes[2],mes[4]);
    }else if(mes[0] == "NEWCOMMENT"){
        _userBroker->addContent(mes[1],mes[3],mes[2],mes[4],mes[5]);
        tranfserComment(mes[1],mes[2],mes[3],mes[4],mes[5]);
    }else if(mes[0] == "GETCOMMENTSIGNAL"){
        sendComment(sock);
    }
    return " ";
}

void Server::userRegister(std::string n, std::string pw,socket_ptr sock)
{
    std::string sql = "insert into user(name,password,friends) values('"
            + n +"','" + pw + "','');";
    std::cout << sql << std::endl;

    //    std::string ssql = "select * from user where name = '" + n +"'";
    if(!_userBroker->selectUser(n))
    {
        if(_userBroker->insertUser(sql))
        {
            std::string message = "REGISTERSUCCEEDED";
            auto s = message.data();
            boost::system::error_code ec;
            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
        }
        else
        {
            std::string message = "REGISTERFAILED";
            auto s = message.data();
            boost::system::error_code ec;
            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
        }
    }
    else {
        std::string message = "REGISTERED";
        auto s = message.data();
        boost::system::error_code ec;
        sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
        std::cout << message << std::endl;
        if(ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return;
        }
    }

}

void Server::userLogin(std::string n, std::string pw, socket_ptr sock)
{
    boost::system::error_code ec;
    auto ep = sock->remote_endpoint(ec);
    if(ec)
    {

        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
    if(_userBroker->selectUser(n))
    {
        if(_userBroker->userLogined(n))
        {
            std::string message = "LOGGINED";
            auto s = message.data();
            boost::system::error_code ec;
            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
            return;
        }

        if(_userBroker->verifyPassword(n,pw))
        {

            std::string friends = _userBroker->friendList(n);
            std::cout << friends << std::endl;
            std::string message = "LOGINSUCCEEDED_";
            message += friends;
            message += "_";
            auto s = message.data();

            boost::system::error_code ec;
            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
            //查看是否有好友请求
            if(!_friendRequest.empty())
            {
                std::string mes = "FRIENDREQUEST";
                bool have = false;
                for(int i = 0; i != _friendRequest.size();i++)
                {
                    std::vector<std::string> names;
                    splictString(_friendRequest[i],names,"_");

                    if(names[0] == n)
                    {
                        mes += "_" + names[1];
                        have = true;
                        _friendRequest[i] = "1";
                    }
                }
                std::vector<std::string> change;
                for(int i = 0; i != _friendRequest.size();i++)
                {
                    if(_friendRequest[i] != "1")
                    {
                        change.push_back(_friendRequest[i]);
                    }
                }
                _friendRequest = change;

                if(have)
                {
                    auto s = mes.data();
                    boost::system::error_code ec;
                    sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
                    std::cout << mes << std::endl;
                    if(ec)
                    {
                        std::cout << boost::system::system_error(ec).what() << std::endl;
                        return;
                    }
                }
            }

            sendDynamic(sock,n);
//            sendComment(sock);

            _userBroker->addLoginUser(n,ep.address().to_string());
            _userBroker->printLoginUser();

            _userBroker->getConversation();
            std::vector<Conversation> mes;
            mes = _userBroker->getTheUserMessage(n);
            //            std::cout << mes[0].getMessage() << std::endl;
            if(!mes.empty())
            {
                outlinttra(mes,sock);
            }
            else
            {
                noMessage(sock);
            }
        }
        else {
            std::string message = "PASSWORDERROR";
            auto s = message.data();
            boost::system::error_code ec;
            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
        }
    }
    else{
        std::string message = "NOACCOUNT";
        auto s = message.data();
        boost::system::error_code ec;
        sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
        std::cout << message << std::endl;
        if(ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return;
        }
    }
}

void Server::conversation(std::string sendName, std::string message, std::string recieveName, socket_ptr sock)
{
    std::cout << "sendName: " << sendName << std::endl;
    std::cout << "message: " << message << std::endl;
    std::cout << "recieveName: " << recieveName << std::endl;

    _userBroker->addConversation(sendName,message,recieveName);

    std::string mes = "SENDMESSAGE";
    auto s = mes.data();
    boost::system::error_code ec;
    sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
    std::cout << mes << std::endl;
    if(ec)
    {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
    waitMessage(sock,recieveName);
}

void Server::tranfserMessage(std::vector<Conversation> mes,socket_ptr sock)
{
    if(!mes.empty())
    {
        std::vector<std::vector<std::string>> friendsMessage;
        for(Conversation c : mes)
        {

            if(friendsMessage.empty())
            {
                std::vector<std::string> tmpMess;
                tmpMess.push_back(c.getSendName());
                tmpMess.push_back(c.getRecieveName());
                tmpMess.push_back(c.getMessage());
                // std::cout << "csscdc" << c.getMessage() << std::endl;
                friendsMessage.push_back(tmpMess);
            }
            else{
                for(int i = 0; i != friendsMessage.size();i++)
                {
                    if(friendsMessage[i][0] == c.getSendName() &&
                            friendsMessage[i][1] == c.getRecieveName())
                    {
                        friendsMessage[i].push_back(c.getMessage());
                    }
                    else
                    {
                        std::vector<std::string> tmpMess;
                        tmpMess.push_back(c.getSendName());
                        tmpMess.push_back(c.getRecieveName());
                        tmpMess.push_back(c.getMessage());
                        friendsMessage.push_back(tmpMess);
                    }
                }
            }
        }

        for(int i = 0; i != friendsMessage.size(); i++)
        {
            std::vector<std::string> tmp;
            tmp = friendsMessage[i];
            std::string message = "RECIEVEMESSAGEFROM";
            message += "_";
            message += tmp[0];
            message += "_";
            message += tmp[2];
            for(int j = 3; j != tmp.size(); j++)
            {
                message += "_";
                message += tmp[j];
            }
            std::cout << message << std::endl;
            auto s = message.data();
            boost::system::error_code ec;

            auto iter = _sock.find(tmp[1]);
            //            tellFriendRecieve(iter->second);

            iter->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            _userBroker->changeMessageStatus(tmp[0]);
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
        }
    }
}

void Server::outlinttra(std::vector<Conversation> mes, socket_ptr sock)
{
    if(!mes.empty())
    {
        std::vector<std::vector<std::string>> friendsMessage;
        for(Conversation c : mes)
        {

            if(friendsMessage.empty())
            {
                std::vector<std::string> tmpMess;
                tmpMess.push_back(c.getSendName());
                tmpMess.push_back(c.getRecieveName());
                tmpMess.push_back(c.getMessage());
                // std::cout << "csscdc" << c.getMessage() << std::endl;
                friendsMessage.push_back(tmpMess);
            }
            else{
                for(int i = 0; i != friendsMessage.size();i++)
                {
                    if(friendsMessage[i][0] == c.getSendName() &&
                            friendsMessage[i][1] == c.getRecieveName())
                    {
                        friendsMessage[i].push_back(c.getMessage());
                    }
                    else
                    {
                        std::vector<std::string> tmpMess;
                        tmpMess.push_back(c.getSendName());
                        tmpMess.push_back(c.getRecieveName());
                        tmpMess.push_back(c.getMessage());
                        friendsMessage.push_back(tmpMess);
                    }

                }
            }
        }

        for(int i = 0; i != friendsMessage.size(); i++)
        {
            std::vector<std::string> tmp;
            tmp = friendsMessage[i];
            std::string message = "RECIEVEMESSAGEFROM";
            message += "_";
            message += tmp[0];
            message += "_";
            message += tmp[2];
            for(int j = 3; j != tmp.size(); j++)
            {
                message += "_";
                message += tmp[j];
            }
            std::cout << message << std::endl;
            auto s = message.data();
            boost::system::error_code ec;

            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            _userBroker->changeMessageStatus(tmp[0]);
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
        }
    }
}

bool Server::waitMessage(socket_ptr sock,std::string n)
{
    std::string res;
    while(1){
        try {
            _userBroker->getConversation();
            std::vector<Conversation> mes;
            mes = _userBroker->getTheUserMessage(n);
            if(_userBroker->selectLoginFriend(n))
            {
                tranfserMessage(mes,sock);
            }
            else
                break;
            char datarec[512];
            memset(datarec,0,sizeof(char) * 512);
            size_t len = sock->read_some(boost::asio::buffer(datarec));

            std::cout << "Message from client:"
                      << datarec << std::endl;
            std::string s = datarec;
            if(strlen(datarec) != 0)
            {
                res = processMessage(s,sock);
                if(res == "EXIT")
                    break;
            }
        }
        catch (boost::system::system_error e)
        {
            std::cout << e.what() << std::endl;
            break;
        }
    }
}

void Server::noMessage(socket_ptr sock)
{
    std::string message = "*NOMESSAGE";
    auto s = message.data();
    boost::system::error_code ec;
    sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
    std::cout << message << std::endl;
    if(ec)
    {
        std::cout << boost::system::system_error(ec).what() << std::endl;
        return;
    }
}


void Server::addfriend(std::string friendname, std::string requestname)
{
    if(_userBroker->selectUser(friendname))
    {
        if(_userBroker->selectLoginFriend(friendname))
        {
            std::string mes = "FRIENDREQUEST_" + requestname;
            mes += "_";
            auto s = mes.data();
            boost::system::error_code ec;
            auto it = _sock.find(friendname);
            it->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << mes << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
        }
        else
        {
            std::string s = friendname + "_" + requestname;
            _friendRequest.push_back(s);
        }
    }
    else{
        std::string mes = "FRIENDONTEXIST";
        auto s = mes.data();
        boost::system::error_code ec;
        auto it = _sock.find(requestname);
        it->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
        std::cout << mes << std::endl;
        if(ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return;
        }

    }
}

void Server::acceptFriend(std::string friendname, std::string requestname)
{
    if(_userBroker->addFriend(friendname,requestname))
    {
        std::string mes = "FRIENDACCEPT_";
        mes += requestname;
        auto s = mes.data();
        boost::system::error_code ec;
        auto it = _sock.find(friendname);
        it->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
        std::cout << mes << std::endl;
        if(ec)
        {
            std::cout << boost::system::system_error(ec).what() << std::endl;
            return;
        }
        std::cout << "tianjia haoyou " << std::endl;
    }
}

void Server::splictString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

void Server::tranfserDynamic(std::string name, std::string content, std::string time)
{
    std::string friendsMsg = _userBroker->friendList(name);
    std::vector<std::string> friends;
    boost::system::error_code ec;
    if(friendsMsg.find(" ")){
        splictString(friendsMsg,friends," ");
    }
    for(int i=0;i!=friends.size();i++){
        for(auto item = _sock.begin();item != _sock.end(); item++){
            auto value = item->first;
            if(value == friends[i]){
                std::string message = "DYNAMIC_"+name+"_"+content+"_"+time;
                auto s = message.data();
                item->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
                if(ec){
                    std::cout << boost::system::system_error(ec).what() << std::endl;
                }
            }
        }
    }
}

void Server::tranfserLike(std::string name, std::string content, std::string time, std::string like)
{
    for(auto item = _sock.begin();item != _sock.end();item++){
        boost::system::error_code ec;
        std::string message = "UPDATELIKE_"+name+"_"+content+"_"+time+"_"+like;
//        std::cout << "!!!!!!!!!!!~~~~~~~~~~~" << message << std::endl;
        auto s = message.data();
        item->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
        if(ec){
            std::cout << boost::system::system_error(ec).what() << std::endl;
        }
    }
}

void Server::tranfserComment(std::string sendName, std::string time, std::string content, std::string commentName, std::string commentary)
{
    for(auto item=_sock.begin();item != _sock.end();item++){
        std::string message = "NEWCOMMENTARY_"+sendName+"_"+time+"_"+content+"_"+commentName+"_"+commentary;
        auto s = message.data();
        boost::system::error_code ec;
        if(item->first != " "){
            item->second->write_some(boost::asio::buffer(s,strlen(s)),ec);
//            std::cout << "wofageile:::" << "[" << item->first << "]" << std::endl;
        }
        if(ec){
            std::cout << boost::system::system_error(ec).what() << std::endl;
        }
    }
}

void Server::oneToOne(std::string sendName,std::string content,std::string time,std::string like,socket_ptr sock)
{
    std::string message = "DYNAMICFROM_"+sendName+"_"+content+"_"+time+"_"+like+"_";
    auto s = message.data();
    boost::system::error_code ec;
    sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
}

void Server::sendDynamic(socket_ptr sock, std::string n)
{
    _userBroker->readDynamic();
    auto dynamic = _userBroker->getDynamic();
    std::string friends = _userBroker->friendList(n);

    std::vector<std::string> myfriend;
    //    std::cout << "::::::::" << friends << std::endl;
    if(friends.find(" ")){
        splictString(friends,myfriend," ");
    }else{
        myfriend.push_back(friends);
    }

    if(dynamic.size()!=0){
        for(auto &d:dynamic){
            for(int i =0;i != myfriend.size();i++){
                if(myfriend[i] == d.getName()){
                    oneToOne(d.getName(),d.getContent(),d.getPushTime(),d.getLike(),sock);
                }
            }
            if(n == d.getName()){
                oneToOne(d.getName(),d.getContent(),d.getPushTime(),d.getLike(),sock);
            }
        }
    }
}

void Server::sendComment(socket_ptr sock)
{
    _userBroker->readComment();
    auto mycomments = _userBroker->getComment();

    std::string message = "COMMENTFROM_";
    if(mycomments.size() != 0){
        for(auto &c:mycomments){
            message += c;
            message += "_";
        }
    }
    boost::system::error_code ec;
    auto s = message.data();
    sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
    if(ec){
        std::cout << boost::system::system_error(ec).what() << std::endl;
    }
}
