#include "userbroker.h"
#include <iostream>
#include "user.h"

using std::cout;    using std::vector;
using std::endl;    using std::string;

std::shared_ptr<UserBroker> UserBroker::_instance = std::make_shared<UserBroker>(UserBroker());

UserBroker::UserBroker()
{

}

void UserBroker::selectAllUsers()
{
    MYSQL *mysql;
    mysql = new MYSQL;

    MYSQL_RES *result;
    MYSQL_ROW row;

    mysql_init(mysql);
    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0))
    {
        cout << "Connect MYSQL failed." << endl;
    }
    else{
        cout << "Connect MYSQL succeed." << endl;
    }

    string sql = "select * from user";
    if(mysql_query(mysql,sql.data())){
        cout << "获取数据失败" << endl;
    }
    else{
        result = mysql_use_result(mysql);
        while(1){
            row = mysql_fetch_row(result);
            if(row == nullptr) break;
            vector<string> userRows;
            for(unsigned int i = 0;i < mysql_num_fields(result);++i)
            {
                userRows.push_back(string(row[i]));

            }
            for(int i = 0; i != userRows.size(); i+=3)
            {
                vector<string> friendsName;
                splictString(userRows[i+2],friendsName,",");
                _users.push_back(User(userRows[0+i],userRows[1+i],friendsName));
            }
        }
//        for(User a : _users)
//        {
//            cout << a.name() << " "
//                 << a.password() << " ";
//            for(string t : a.friends())
//            {
//                cout << t << " ";
//            }
//            cout << endl;
//        }
        mysql_free_result(result);
        result = nullptr;
    }
    if(mysql != nullptr)
        mysql_close(mysql);
    mysql_library_end();
}

bool UserBroker::insertUser(const std::string &sql)
{
    MYSQL *mysql;
    mysql = new MYSQL;

    mysql_init(mysql);
    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0))
    {
        cout << "Connect MYSQL failed." << endl;
    }
    else{
        cout << "Connect MYSQL successed." << endl;
    }

    if(mysql_query(mysql,sql.data())){
        cout << "更新数据失败" << endl;
        return false;
    }
    else{
        return true;
    }

    if(mysql != nullptr)
        mysql_close(mysql);
    mysql_library_end();
}

bool UserBroker::selectUser(const std::string &n)
{
    _users.clear();
    selectAllUsers();
   for(User t : _users)
   {
       if(t.name() == n)
       {
           return true;
       }
   }
   return false;
}

bool UserBroker::verifyPassword(const std::string n, const std::string pw)
{
    for(User t : _users)
    {
        if(t.name() == n)
        {
            if(t.password() == pw)
                return true;
        }
    }
    return false;
}

void UserBroker::addLoginUser(std::string n, std::string ip)
{
    for(User t : _users)
    {
        if(t.name() == n)
        {
           User a = t;
           a.setIp(ip);
           _loginUsers.push_back(a);
        }
    }
}

void UserBroker::printLoginUser()
{
    for(User t : _loginUsers)
    {
        std::cout << "-----------------" << std::endl;
        std::cout << t.name() << " " << t.ip() << " 在线" << std::endl;
    }
    std::cout << _loginUsers.size() << std::endl;
}

void UserBroker::splictString(const std::string &s, std::vector<std::string> &v, const std::string &c)
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

std::vector<User> UserBroker::getUsers() const
{
    return _users;
}
