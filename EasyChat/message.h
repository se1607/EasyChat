#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message
{

public:
    Message();

    QString getMess() const;
    void setMess(const QString &value);

    bool getWho() const;
    void setWho(bool value);

private:
    QString mess;
    bool who;
};

#endif // MESSAGE_H
