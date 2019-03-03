#ifndef USERBROKER_H
#define USERBROKER_H

#include "relationalbroker.h"
#include <map>

class User;

class UserBroker : public RelationalBroker
{
public:
    static std::shared_ptr<UserBroker> getInstance()
    {
        return _instance;
    }

    void selectAllUsers();
    bool insertUser(const std::string &sql);
    bool selectUser(const std::string &n);
    bool verifyPassword(const std::string n,const std::string pw);
    void addLoginUser(std::string n,std::string ip);
    void printLoginUser();
    void splictString(const std::string& s, std::vector<std::string>& v, const std::string& c);

    std::vector<User> getUsers() const;

private:
    UserBroker();
    static std::shared_ptr<UserBroker> _instance;
    std::vector<User> _users;
    std::vector<User> _loginUsers;
};

#endif // USERBROKER_H
