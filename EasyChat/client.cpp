#include <iostream>
#include "client.h"

boost::asio::io_service service;
boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("192.168.30.141"),PORT);
boost::asio::ip::tcp::socket sock(service);
boost::asio::ip::tcp::socket sock_fileTransfer(service);

Client::Client()
{

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
