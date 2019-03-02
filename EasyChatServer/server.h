#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#define PORT 6666

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;

class Server
{
public:
    Server();

    void acceptConnect();
    void receiveMessage(socket_ptr sock);
};

#endif // SERVER_H
