#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User
{
public:
    User();
    User(std::string name,std::string password);
    User(std::string name,std::string password,std::vector<std::string> friends);
    std::string name() const;

    std::string password() const;

    std::vector<std::string> friends() const;

    std::string ip() const;
    void setIp(const std::string &ip);

private:
    std::string m_name;
    std::string m_password;
    std::vector<std::string> m_friends;
    std::string m_ip;
};

#endif // USER_H
