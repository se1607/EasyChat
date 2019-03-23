#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "userbroker.h"
#include <map>

#define PORT 6666

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class Server
{
public:
    Server();

    void acceptConnect();
    void receiveMessage(socket_ptr sock);
    std::string processMessage(std::string message,socket_ptr sock);

    void userRegister(std::string n, std::string pw, socket_ptr sock);
    void userLogin(std::string n, std::string pw, socket_ptr sock);
    void conversation(std::string sendName,std::string message,
                      std::string recieveName,socket_ptr sock);
    void tranfserMessage(std::vector<Conversation> mes,socket_ptr sock);
    void outlinttra(std::vector<Conversation> mes,socket_ptr sock);
    bool waitMessage(socket_ptr sock, std::string n);
    void noMessage(socket_ptr sock);

    void addfriend(std::string friendname,std::string requestname);

    void acceptFriend(std::string friendname,std::string requestname);

    void splictString(const std::string& s, std::vector<std::string>& v, const std::string& c);

private:
    std::shared_ptr<UserBroker> _userBroker;
    std::map<std::string,socket_ptr> _sock;
//    std::map<std::string,std::string> _friendRequest;
    std::vector<std::string> _friendRequest;
};

#endif // SERVER_H
