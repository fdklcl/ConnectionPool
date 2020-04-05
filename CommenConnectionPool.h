#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
#include <thread>
#include <functional>
#include <condition_variable>
#include "Connection.h"
using namespace std;
/*
�̰߳�ȫ����������ģʽ
*/
class ConnectionPool
{
public:
	//��ȡ���ӳض���ʵ��
	static ConnectionPool* getConnectionPool()
	{
		//��̬�ֲ��������е��˴���ʱ��Ų������� �Զ�lock()��unlock()����
		static ConnectionPool pool;  
		return &pool;
	}

	//�����ӳ��л�ȡһ����������
	shared_ptr<Connection> getConnection();
private:
	ConnectionPool();  //���캯��˽�л�

	//�������ļ��м���������
	bool loadConfigFile();

	//�����ڶ������߳���,ר�Ÿ�������������
	void produceConnectionTask();

	//ɨ�賬��maxIdleTimeʱ��Ŀ�������,���ж�������ӻ���
	void scannerConnectionTask();

	string _ip;               //mysql��ip��ַ
	unsigned short _port;     //mysql�Ķ˿ں�,Ĭ��3306
	string _username;         //mysql��¼�û���
	string _password;         //mysql��¼����
	string _dbname;           //mysql���ݿ�����
	int _initSize;            //���ӳصĳ�ʼ������
	int _maxSize;             //���ӳص����������
	int _maxIdleTime;         //���ӳص����ռ�ʱ��
	int _connectionTimeout;   //���ӳػ�ȡ���ӵ����ʱʱ��

	queue<Connection*> _connectionQue;   //�洢mysql���ӵĶ���
	mutex _queueMutex;         //ά�����Ӷ��е��̰߳�ȫ�Ļ�����,��֤_connectionQueue���̰߳�ȫ
	atomic_int _connectionCnt; //��¼������������connection���ӵ�������,֧���̰߳�ȫ
	condition_variable cv;     //������������,�����������̺߳��������߳�֮���ͨ��
};