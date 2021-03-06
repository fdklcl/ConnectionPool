#pragma once
#include <iostream>
#include <mysql.h>
#include <ctime>
#include <string>
using namespace std;
/*实现数据库的操作*/

class Connection
{
public:
	Connection();
	~Connection();
	bool connect(string ip,
		unsigned short port,
		string user,
		string password,
		string dbname);
	bool update(string sql);
	MYSQL_RES* query(string sql);
	//刷新一下连接的起始的空闲时间
	void refreshAliveTime() { _alivetime = clock(); }
	//返回存活时间
	clock_t getAliveTime() const { return clock() - _alivetime; }
private:
	MYSQL* _conn;           //表示和MYSQL的一条连接

	/*
	clock_t
	*/
	clock_t _alivetime;    //记录进入空闲状态后的存活时间
};