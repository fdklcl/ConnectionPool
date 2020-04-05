#include "CommenConnectionPool.h"
#include "public.h"
#include <Cstring>

//�������ļ��м���������
bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");
	if (nullptr == pf)
	{
		LOG("mysql.ini file is not exist!");
		return false;
	}

	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		string str = line;
		int idx = str.find('=', 0);
		if (idx == -1) //��Ч������
		continue;

		int endidx = str.find('\n', idx);
		//substr(start,length)����һ����ָ��λ�ÿ�ʼ��������ָ�����ȵ����ַ�����
		string key = str.substr(0, idx);  
		string value = str.substr((idx + 1), (endidx - idx - 1));

		if (key == "ip")
		{
			_ip = value;
		}
		else if (key == "port")
		{
			_port = atoi(value.c_str());
		}
		else if (key == "username")
		{
			_username = value;
		}
		else if (key == "password")
		{
			_password = value;
		}
		else if (key == "dbname")
		{
			_dbname = value;
		}
		else if (key == "initSize")
		{
			_initSize = atoi(value.c_str());
		}
		else if (key == "maxSize")
		{
			_maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime")
		{
			_maxIdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeout")
		{
			_connectionTimeout = atoi(value.c_str());
		}
	}
}

//���ӳصĹ���
ConnectionPool::ConnectionPool()
{
	//����������
	if (!loadConfigFile())
		return;

	//������ʼ����������
	for (int i = 0; i < _initSize; ++i)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();    //ˢ��һ�¿�ʼ���е���ʼʱ��
		_connectionQue.push(p);
		_connectionCnt++;
	}

	//����һ�����߳�,��Ϊ���ӵ�������
	/*
	�̺߳�������C�ӿ�,produceConnectionTask����ͨ�ĳ�Ա����,��
	��Ա�����ĵ�����Ҫ��������,�����Ҫʹ�ð���
	*/
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));  
	produce.detach();

	//����һ���µĶ�ʱ�߳�,ɨ�賬��maxIdleTimeʱ��Ŀ�������,���ж�������ӻ���
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}

//�����ڶ������߳���,ר�Ÿ�������������
void ConnectionPool::produceConnectionTask()
{
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			//�ȴ����� => �������� =>��ȡ���������ܼ�������
			cv.wait(lock);   //#1�ȴ�״̬ #2�ͷŻ�����
		}

		//������Ϊ��,��������û�е�������,���������µ�����
		if (_initSize < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();    //ˢ��һ�¿�ʼ���е���ʼʱ��
			_connectionQue.push(p);
			_connectionCnt++;
		}

		//֪ͨ�������߳̿��������� �ȴ�״̬=>����״̬
		cv.notify_all();
	}
}

//���ⲿ�ṩһ���ӿ�.�����ӳ��л�ȡһ����������
shared_ptr<Connection> ConnectionPool::getConnection()
{
	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty())
	{
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)))
		{
			if (_connectionQue.empty())
			{
				LOG("��ȡ���ӳ�ʱ...��ȡ����ʧ��");
				return nullptr;
			}
		}
	}
	/*
	shared_ptr����ָ������ʱ,���connection��Դֱ��delete��,�൱�ڵ�
	��connection����������,�ر�����,������ʵ���ϲ�ϣ�������Ĺر�����,
	������ʹ�����,�����ӹ黹�����ӳ���,�����Ҫ�Զ�������ָ���ɾ����
	*/
	shared_ptr<Connection> sp(_connectionQue.front()
		, [&](Connection* pcon) 
	{
		unique_lock<mutex> lock(_queueMutex);
		pcon->refreshAliveTime();    //ˢ��һ�¿�ʼ���е���ʼʱ��
		_connectionQue.push(pcon);
	});
	_connectionQue.pop();
	cv.notify_all();  //�����������Ժ�,֪ͨ�������̼߳�����,��Ϊ��,��������
	
	return sp;
}

//ɨ�賬��maxIdleTimeʱ��Ŀ�������, ���ж�������ӻ���
void ConnectionPool::scannerConnectionTask()
{
	for (;;)
	{
		//ͨ��sleepģ�ⶨʱЧ��
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));

		//ɨ����������,�ͷŶ��ڵ�����
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();
			if (p->getAliveTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p;    //����~Connection()�ͷ�����
			}
			else
			{
				break;       //��ͷ������û�г���_maxIdleTime,���������ӿ϶�û�г���
			}
		}
	}
}