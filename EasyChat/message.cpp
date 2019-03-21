#include "message.h"

Message::Message()
{

}

QString Message::getMess() const
{
    return mess;
}

void Message::setMess(const QString &value)
{
    mess = value;
}

bool Message::getWho() const
{
    return who;
}

void Message::setWho(bool value)
{
    who = value;
}


