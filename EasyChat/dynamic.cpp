#include "dynamic.h"

Dynamic::Dynamic()
{

}

void Dynamic::splictString(const std::string &s, std::vector<std::string> &v, const std::string &c)
{
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

QString Dynamic::getName()
{
    return pushName;
}

QString Dynamic::getContent()
{
    return pushContent;
}

QString Dynamic::getPushTime()
{
    return pushTime;
}

QString Dynamic::getLike()
{
    return like;
}

void Dynamic::setPushName(const QString n)
{
    pushName = n;
}

void Dynamic::setPushContent(const QString n)
{
    pushContent = n;
}

void Dynamic::setPushTime(const QString n)
{
    pushTime = n;
}

void Dynamic::setLike(QString v)
{
    like = v;
}
