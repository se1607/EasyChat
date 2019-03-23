#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <string>

class Conversation
{
public:
    Conversation();


    std::string getSendName() const;
    void setSendName(const std::string &value);

    std::string getMessage() const;
    void setMessage(const std::string &value);

    std::string getRecieveName() const;
    void setRecieveName(const std::string &value);

    bool getTranfser() const;
    void setTranfser(bool value);

private:
    std::string sendName;
    std::string message;
    std::string recieveName;
    bool tranfser;
};

#endif // CONVERSATION_H
