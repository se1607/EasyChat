#include "conversation.h"

Conversation::Conversation()
{

}

QString Conversation::getName() const
{
    return name;
}

void Conversation::setName(const QString &value)
{
    name = value;
}

QStringList Conversation::getMesageList() const
{
    return mesageList;
}

void Conversation::setMesageList(const QStringList &value)
{
    mesageList = value;
}
