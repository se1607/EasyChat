#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "userbroker.h"

#define PORT 6666

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class Server
{
public:
    Server();

    void acceptConnect();
    void receiveMessage(socket_ptr sock);
    void processMessage(std::string message,socket_ptr sock);

    void userRegister(std::string n, std::string pw, socket_ptr sock);

    void splictString(const std::string& s, std::vector<std::string>& v, const std::string& c);

private:
    std::shared_ptr<UserBroker> _userBroker;
};

#endif // SERVER_H
