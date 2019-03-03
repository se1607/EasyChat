#include <iostream>
#include "client.h"

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

void Client::receiveMessage()
{
    boost::system::error_code ec;

    try {
        char data[512];
        memset(data,0,sizeof(char) * 512);
        size_t len = sock.read_some(boost::asio::buffer(data));
        std::string s = data;
//        std::cout << "Message from server:"
//                  << s << std::endl;

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


void Client::processMessage(std::string message)
{
    if(message == "REGISTERSUCCEEDED"){
        emit registersucceeded();
    }else if(message == "REGISTERFAILED"){
        emit registerfailed();
    }else if(message == "REGISTERED"){
        emit registered();
    }else if(message == "LOGINSUCCEEDED"){
        emit logined();
    }else if(message == "NOACCOUNT"){
        emit loginnameerror();
    }else if(message == "PASSWORDERROR"){
        emit loginpassworderror();
    }
}
