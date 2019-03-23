#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QString>
#include <QStringList>
#include "message.h"
#include <vector>

class Conversation
{
public:
    Conversation();

    QString getName() const;
    void setName(const QString &value);

    QStringList getMesageList() const;
    void setMesageList(const QStringList &value);

    void appendMessage(QString s);

    void appendRecod(QString s, bool b, std::string sendName, std::string recieve,std::string loginName);

    std::vector<Message> getConverRecord() const;

    void setConverRecord(const std::vector<Message> &value);

    void writeFile(std::string sendname, std::string recievename, std::string message,std::string loginName);

private:
    QString friendname;
    QStringList mesageList;
    std::vector<Message> converRecord;
};

#endif // CONVERSATION_H
