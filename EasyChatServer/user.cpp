#include "user.h"

User::User()
{

}

User::User(std::string name, std::string password)
{
    m_name = name;
    m_password = password;
}

User::User(std::string name, std::string password, std::vector<std::string> friends)
{
    m_name = name;
    m_password = password;
    m_friends = friends;
}

std::string User::name() const
{
    return m_name;
}

std::string User::password() const
{
    return m_password;
}

std::vector<std::string> User::friends() const
{
    return m_friends;
}

std::string User::ip() const
{
    return m_ip;
}

void User::setIp(const std::string &ip)
{
    m_ip = ip;
}
