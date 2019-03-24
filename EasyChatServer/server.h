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

    void splictString(const std::string& s, std::vector<std::string>& v, const std::string& c);

    void tranfserDynamic(std::string name,std::string content,std::string time);
    void tranfserLike(std::string name,std::string content,std::string time,std::string like);
    void tranfserComment(std::string sendName,std::string time,std::string content,std::string commentName,std::string commentary);
    void sendDynamic(socket_ptr sock,std::string n);
    void sendComment(socket_ptr sock);
    void oneToOne(std::string sendName, std::string content, std::string time, std::string like, socket_ptr sock);
//    void haveReadSignal(socket_ptr sock);
private:
    std::shared_ptr<UserBroker> _userBroker;
    std::map<std::string,socket_ptr> _sock;
};

#endif // SERVER_H
