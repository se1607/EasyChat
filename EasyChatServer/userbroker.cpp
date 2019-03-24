#include "userbroker.h"
#include <iostream>
#include "user.h"
#include "conversation.h"

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
//    mysql_library_end();
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
//    mysql_library_end();
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

bool UserBroker::selectLoginFriend(const std::string n)
{
    for(User t : _loginUsers)
    {
        if(t.name() == n)
        {
            return true;
        }
    }
    return false;
}

std::string UserBroker::friendList(const std::string n)
{
    std::string friends;
    for(User t : _users)
    {
        if(t.name() == n)
        {
            std::vector<std::string> tmp = t.friends();
            for(std::string a : tmp)
            {
                friends += a + " ";
            }
            return friends;
        }
    }
    return " ";
}

std::string UserBroker::friendIp(const std::string n)
{
    for(User t : _loginUsers)
    {
        if(t.name() == n)
        {
            return t.ip();
        }
    }
    return " ";
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

void UserBroker::deletLoginUser(std::string n)
{
    for(auto it = _loginUsers.begin();it != _loginUsers.end();)
    {
        User t = *it;
        if(t.name() == n){

            it = _loginUsers.erase(it);
            std::cout << t.name() << " 退出登录" << std::endl;
        }
        else {
            ++it;
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

bool UserBroker::addConversation(std::string send, std::string mes, std::string recieve)
{
    std::string sql = "insert into conversation(sendName,message,recieveName,tranfser) values('"
            + send +"','" + mes + "','" + recieve + "'," + "'0');";
    std::cout << sql << std::endl;

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
//    mysql_library_end();
}

void UserBroker::getConversation()
{
    _conversation.clear();
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

    string sql = "select * from conversation;";
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
            for(int i = 0; i != userRows.size(); i+=4)
            {
               Conversation c;
               c.setSendName(userRows[0+i]);
               c.setMessage(userRows[1+i]);
               c.setRecieveName(userRows[2+i]);
               if(userRows[3+i] == "0")
                   c.setTranfser(false);
               else
                   c.setTranfser(true);
               _conversation.push_back(c);
            }
        }
//        for(Conversation c : _conversation)
//        {
//            cout << c.getMessage() << " ";

//            cout << endl;
//        }
        mysql_free_result(result);
        result = nullptr;
    }
    if(mysql != nullptr)
        mysql_close(mysql);
//    mysql_library_end();
}

std::vector<Conversation> UserBroker::getTheUserMessage(std::string name)
{
    std::vector<Conversation> mes;
    for(Conversation c : _conversation)
    {
        if(c.getRecieveName() == name && !c.getTranfser())
        {
            mes.push_back(c);
        }
    }
    return mes;
}

bool UserBroker::changeMessageStatus(std::string name)
{
    std::string sql = "update conversation set tranfser = '1' where sendName = '" + name + "';";
    std::cout << sql << std::endl;

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
        cout << "修改消息状态失败" << endl;
        return false;
    }
    else{
        return true;
    }

    if(mysql != nullptr)
        mysql_close(mysql);
//    mysql_library_end();
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

bool UserBroker::addDynamic(std::string name, std::string content, std::string time)
{
    std::string like="";
    std::string sql = "insert into dynamic(sendName,content,time,liking) values('"+name
            +"','"+content+"','"+time+"','"+like+"');";
    MYSQL *mysql;
    mysql = new MYSQL;

    mysql_init(mysql);
    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0)){
        cout << "Connect MYSQL failed." << endl;
    }else{
        cout << "Connect MYSQL succeed" << endl;
    }

    if(mysql_query(mysql,sql.data())){
        cout << "Failed" << endl;
        return false;
    }else{
        return true;
    }

    if(mysql != nullptr)
        mysql_close(mysql);
}

bool UserBroker::addContent(std::string sendName, std::string content, std::string time, std::string contentName, std::string contentary)
{
    std::string sql = "insert into comments(sendName,content,pushTime,commentName,commentary) values('"+sendName
            +"','"+content+"','"+time+"','"+contentName+"','"+ contentary+"');";
    MYSQL *mysql;
    mysql = new MYSQL;

    mysql_init(mysql);
    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0)){
        cout << "Connect MYSQL failed." << endl;
    }else{
        cout << "Connect MYSQL successed." << endl;
    }

    if(mysql_query(mysql,sql.data())){
        cout << "Failed" << endl;
    }else{
        return true;
    }

    if(mysql != nullptr)
        mysql_close(mysql);
}

void UserBroker::readDynamic()
{
    _dynamic.clear();
    MYSQL *mysql;
    mysql = new MYSQL;

    MYSQL_RES *result;
    MYSQL_ROW row;

    mysql_init(mysql);
    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0)){
        cout << "failed" << endl;
    }else{
        cout << "succeed" <<endl;
    }

    std::string sql = "select * from dynamic;";
    if(mysql_query(mysql,sql.data())){
        cout << "Get data failed." << endl;
    }else{
        result = mysql_use_result(mysql);
        while(1){
            row = mysql_fetch_row(result);
            if(row == nullptr) break;
            vector<string> userRows;
            for(unsigned int i=0;i<mysql_num_fields(result);++i){
                userRows.push_back(string(row[i]));
            }
            for(int i=0;i != userRows.size(); i+=4){
                Dynamic d;
                d.setPushName(userRows[0+i]);
                d.setPushContent(userRows[1+i]);
                d.setPushTime(userRows[2+i]);
                d.setLike(userRows[3+i]);
                _dynamic.push_back(d);
            }
        }
        mysql_free_result(result);
        result = nullptr;
    }
    if(mysql != nullptr)
        mysql_close(mysql);
}

void UserBroker::readComment()
{
    _comments.clear();
    MYSQL *mysql;
    mysql = new MYSQL;

    MYSQL_RES *result;
    MYSQL_ROW row;

    mysql_init(mysql);

    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0)){
        cout << "failed" << endl;
    }else{
        cout << "succeed" << endl;
    }

    string sql = "select * from comments";
    if(mysql_query(mysql,sql.data())){
        cout << "Get comments data failed" << endl;
    }else{
        result = mysql_use_result(mysql);
        while(1){
            row = mysql_fetch_row(result);
            if(row == nullptr) break;
            vector<string> useRows;
            for(unsigned i=0;i<mysql_num_fields(result);++i){
                useRows.push_back(row[i]);
            }
            for(int i=0;i != useRows.size();i+=5){
                string comment;
                comment = useRows[0+i]+"_"+useRows[1+i]+"_"
                        +useRows[2+i]+"_"+useRows[3+i]+"_"+useRows[4+i];
                cout << comment << endl;
                _comments.push_back(comment);
            }
        }
        mysql_free_result(result);
        result = nullptr;
    }
    if(mysql != nullptr){
        mysql_close(mysql);
    }
}

bool UserBroker::updateDynamic(std::string name, std::string content, std::string time, std::string like)
{
//    cout << name << "wo shou dao le ma" << content << "---" << like << "---" << endl;
    std::string sql = "update dynamic set liking='"+like+"' where content='"+content+"' and sendName='"+name+"' and time='"+time+"';";
    MYSQL *mysql;
    mysql = new MYSQL;
    mysql_init(mysql);
    if(!mysql_real_connect(mysql,"localhost","root","root","EasyChat",0,NULL,0)){
        cout << "connect MYSQL failed" << endl;
    }else{
        cout << "connect MYSQL succeed" << endl;
    }
    if(mysql_query(mysql,sql.data())){
        cout << "query failed" << endl;
        return false;
    }else{
        return true;
    }
    if(mysql != nullptr){
        mysql_close(mysql);
    }
}

std::vector<Dynamic> UserBroker::getDynamic()
{
    return _dynamic;
}

std::vector<std::string> UserBroker::getComment()
{
    return _comments;
}
