#include <iostream>
#include "server.h"
#include "user.h"
#include <string>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(),PORT);
boost::asio::ip::tcp::acceptor acc(service,ep);

Server::Server()
{
    _userBroker = UserBroker::getInstance();
    _userBroker->selectAllUsers();
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
                processMessage(s,sock);
            }
        } catch (boost::system::system_error e)
        {
            std::cout << e.what() << std::endl;
            break;
        }
    }
}

void Server::processMessage(std::string message, socket_ptr sock)
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
    }
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
            std::string message = "LOGINSUCCEEDED";
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
