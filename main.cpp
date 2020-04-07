#include "Connection.h"
#include "CommenConnectionPool.h"

int main()
{
	Connection conn;
	conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
	clock_t begin = clock();
	thread t1([]() {
		for (int i = 0; i < 250; ++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
	});
	thread t2([]() {
		for (int i = 0; i < 250; ++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
	});
	thread t3([]() {
		for (int i = 0; i < 250; ++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
	});
	thread t4([]() {
		for (int i = 0; i < 250; ++i)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
			conn.update(sql);
		}
	});
#if 0
	thread t1([]() {
		ConnectionPool* p = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i)
		{
			shared_ptr<Connection> sp = p->getConnection();
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			sp->update(sql);
		}
	});
	thread t2([]() {
		ConnectionPool* p = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i)
		{
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			shared_ptr<Connection> sp = p->getConnection();
			sp->update(sql);
		}
	});
	thread t3([]() {
		ConnectionPool* p = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i)
		{
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			shared_ptr<Connection> sp = p->getConnection();
			sp->update(sql);
		}
	});
	thread t4([]() {
		ConnectionPool* p = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 250; ++i)
		{
			char sql[1024] = { 0 };
			sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
			shared_ptr<Connection> sp = p->getConnection();
			sp->update(sql);
		}
	});
#endif
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	clock_t end = clock();
	cout << end - begin << "ms" << endl;

#if 0
	for (int i = 0; i < 1000; ++i)
	{
		/*Connection conn;
		char sql[1024] = { 0 };
		sprintf(sql,"insert into sn(id,name,age,sex) values('%s','%s','%d','%s');","i","zhangsan",20,"male");
		conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
		conn.update(sql);*/
		shared_ptr<Connection> sp = p->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "i", "zhangsan", 20, "male");
		sp->update(sql);
	}
#endif
	return 0;
}