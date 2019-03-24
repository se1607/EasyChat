#include "conversation.h"
#include <fstream>
#include <iostream>


Conversation::Conversation()
{

}

QString Conversation::getName() const
{
    return friendname;

}

void Conversation::setName(const QString &value)
{

    friendname = value;
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

void Conversation::appendRecod(QString s, bool b, std::string sendName, std::string recieve, std::string loginName)
{
    writeFile(sendName,recieve,s.toStdString(),loginName);
    Message m;
    m.setMess(s);
    m.setWho(b);
    converRecord.push_back(m);
}

std::vector<Message> Conversation::getConverRecord() const
{
    return converRecord;
}

void Conversation::setConverRecord(const std::vector<Message> &value)
{
    converRecord = value;
}

void Conversation::writeFile(std::string sendname, std::string recievename, std::string message,std::string loginName)
{
    std::string command = "./" + loginName + "/";

    std::string fileName;

    if(loginName == sendname)
    {
        fileName = command + recievename;
    }
    else {
        fileName = command + sendname;
    }

    std::cout << fileName << std::endl;

    std::ifstream file;
    file.open(fileName.data(),std::ios::in);
    if(file)
    {
        std::cout << "文件存在" << std::endl;
        file.close();
        std::ofstream write(fileName,std::ios::app);
        std::string s;
        s += sendname + " ";
        s += recievename + " ";
        s += message + "\n";
        write << s << std::endl;
        std::cout << s << std::endl;
        write.close();
    }
    else {
        std::cout<< "文件bu存在" << std::endl;
        std::ofstream write(fileName.data(),std::ios::app);
        std::string s;
        s += sendname + " ";
        s += recievename + " ";
        s += message;
        write << s << std::endl;
        std::cout << s << std::endl;
        write.close();

        std::cout << "再写一个文件" << std::endl;
        std::ofstream w(sendname.data(),std::ios::app);
        std::string x;
        x += sendname + " ";
        x += recievename + " ";
        x += message;
        write << x << std::endl;
        std::cout << "第二个文件： " << x << std::endl;
        write.close();
    }
}

