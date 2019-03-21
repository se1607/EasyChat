#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QString>
#include <QStringList>

class Conversation
{
public:
    Conversation();

    QString getName() const;
    void setName(const QString &value);

    QStringList getMesageList() const;
    void setMesageList(const QStringList &value);

private:
    QString name;
    QStringList mesageList;
};

#endif // CONVERSATION_H
