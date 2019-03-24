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

std::string Dynamic::getName()
{
    return pushName;
}

std::string Dynamic::getContent()
{
    return pushContent;
}

std::string Dynamic::getPushTime()
{
    return pushTime;
}

std::string Dynamic::getLike()
{
    return like;
}

void Dynamic::setPushName(std::string n)
{
    pushName = n;
}

void Dynamic::setPushContent(std::string n)
{
    pushContent = n;
}

void Dynamic::setPushTime(std::string n)
{
    pushTime = n;
}

void Dynamic::setLike(std::string v)
{
        like = v;
}
