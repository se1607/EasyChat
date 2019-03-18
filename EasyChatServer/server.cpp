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
        if(_userBroker->verifyPassword(n,pw))
        {

            std::string friends = _userBroker->friendList(n);
            std::cout << friends << std::endl;
            std::string message = "LOGINSUCCEEDED_";
            message += friends;
            auto s = message.data();

            boost::system::error_code ec;
            sock->write_some(boost::asio::buffer(s,strlen(s)),ec);
            std::cout << message << std::endl;
            if(ec)
            {
                std::cout << boost::system::system_error(ec).what() << std::endl;
                return;
            }
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
    std::string message = "NOMESSAGE";
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
