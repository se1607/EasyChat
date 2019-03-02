#include <iostream>
#include "server.h"

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(),PORT);
boost::asio::ip::tcp::acceptor acc(service,ep);

Server::Server()
{

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
    std::cout << ep.address().to_string()
              << "已连接" << std::endl;
}
