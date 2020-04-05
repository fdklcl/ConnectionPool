#pragma once
#include <iostream>
#include <mysql.h>
#include <ctime>
#include <string>
using namespace std;
/*ʵ�����ݿ�Ĳ���*/

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
	//ˢ��һ�����ӵ���ʼ�Ŀ���ʱ��
	void refreshAliveTime() { _alivetime = clock(); }
	//���ش��ʱ��
	clock_t getAliveTime() const { return clock() - _alivetime; }
private:
	MYSQL* _conn;           //��ʾ��MYSQL��һ������

	/*
	clock_t
	*/
	clock_t _alivetime;    //��¼�������״̬��Ĵ��ʱ��
};