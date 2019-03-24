#include "relationalbroker.h"
/*
 * create table user(name varchar(20) NOT NULL,password varchar(20) NOT NULL,friends varchar(50));
 * insert into user(name,password,friends) values('123','123','');
 *
 * create table conversation(sendName varchar(20) NOT NULL,message varchar(100) NOT NULL,recieveName varchar(20),tranfser varchar(10));
 *
 * create table comments(sendName varchar(20),content varchar(100),pushTime varchar(20),commentName varchar(20),commentary varchar(100));
 *
 * create table dynamic(sendName varchar(20) NOT NULL,content varchar(100) NOT NULL,time varchar(20) NOT NULL,liking varchar(50));
 *
*/

RelationalBroker::RelationalBroker()
{

}
