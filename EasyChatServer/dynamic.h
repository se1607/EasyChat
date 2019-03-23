#ifndef DYNAMIC_H
#define DYNAMIC_H
#include <QObject>
#include <QStringList>
#include <map>
class Dynamic
{
public:
    Dynamic();
    void setPushName(std::string n);
    void setPushContent(std::string n);
    void setPushTime(std::string n);
    void setComments(const std::string n);
    void setLike(std::string v);
    void splictString(const std::string &s, std::vector<std::string> &v, const std::string &c);
    std::string getName();
    std::string getContent();
    std::string getPushTime();
    std::string getLike();
private:
    std::string pushName;
    std::string pushContent;
    std::string pushTime;
    std::string like;
};
#endif // DYNAMIC_H
