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

void Conversation::appendMessage(QString s)
{
    mesageList.append(s);
}

void Conversation::appendRecod(QString s, bool b)
{
    Message m;
    m.setMess(s);
    m.setWho(b);
    converRecord.push_back(m);
}

std::vector<Message> Conversation::getConverRecord() const
{
    return converRecord;
}
