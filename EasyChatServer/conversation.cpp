#include "conversation.h"

Conversation::Conversation()
{

}

std::string Conversation::getSendName() const
{
    return sendName;
}

void Conversation::setSendName(const std::string &value)
{
    sendName = value;
}

std::string Conversation::getMessage() const
{
    return message;
}

void Conversation::setMessage(const std::string &value)
{
    message = value;
}

std::string Conversation::getRecieveName() const
{
    return recieveName;
}

void Conversation::setRecieveName(const std::string &value)
{
    recieveName = value;
}

bool Conversation::getTranfser() const
{
    return tranfser;
}

void Conversation::setTranfser(bool value)
{
    tranfser = value;
}
