#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

#define PORT 6666

class Client
{
public:
    Client();

    void connectServer();
};

#endif // CLIENT_H
