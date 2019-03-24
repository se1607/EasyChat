#ifndef DYNAMIC_H
#define DYNAMIC_H
#include <QObject>
#include <QStringList>
#include <map>
class Dynamic
{
public:
    Dynamic();
    void setPushName(const QString n);
    void setPushContent(const QString n);
    void setPushTime(const QString n);
    void setComments(const std::string n);
    void setLike(QString v);
    void splictString(const std::string &s, std::vector<std::string> &v, const std::string &c);
    QString getName();
    QString getContent();
    QString getPushTime();
    QString getLike();
private:
    QString pushName;
    QString pushContent;
    QString pushTime;
    QString like;
};

#endif // DYNAMIC_H
