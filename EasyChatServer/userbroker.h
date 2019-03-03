#ifndef USERBROKER_H
#define USERBROKER_H

#include "relationalbroker.h"

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
    void splictString(const std::string& s, std::vector<std::string>& v, const std::string& c);

    std::vector<User> getUsers() const;

private:
    UserBroker();
    static std::shared_ptr<UserBroker> _instance;
    std::vector<User> _users;
};

#endif // USERBROKER_H
