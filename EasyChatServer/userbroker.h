#ifndef USERBROKER_H
#define USERBROKER_H

#include "relationalbroker.h"
#include <map>
#include "dynamic.h"

class User;
class Conversation;

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

    bool selectLoginFriend(const std::string n);

    std::string friendList(const std::string n);
    std::string friendIp(const std::string n);

    void addLoginUser(std::string n,std::string ip);
    void deletLoginUser(std::string n);

    bool userLogined(std::string n);

    void printLoginUser();

    bool addConversation(std::string send,std::string mes,std::string recieve);
    void getConversation();

    std::vector<Conversation> getTheUserMessage(std::string name);
    bool changeMessageStatus(std::string name);

    void splictString(const std::string& s, std::vector<std::string>& v, const std::string& c);

    std::vector<User> getUsers() const;

    bool addFriend(std::string friendname,std::string requestname);

    bool addDynamic(std::string name,std::string content,std::string time);
    bool addContent(std::string sendName, std::string content, std::string time, std::string contentName, std::string contentary);

    void readDynamic();
    void readComment();
    bool updateDynamic(std::string name,std::string content,std::string time,std::string like);
    std::vector<Dynamic> getDynamic();
    std::vector<std::string> getComment();

private:
    UserBroker();
    static std::shared_ptr<UserBroker> _instance;
    std::vector<User> _users;
    std::vector<User> _loginUsers;
    std::vector<Conversation> _conversation;
    std::vector<Dynamic> _dynamic;
    std::vector<std::string> _comments;
};

#endif // USERBROKER_H
