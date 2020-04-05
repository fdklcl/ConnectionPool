#include "Connection.h"
#include "CommenConnectionPool.h"

int main()
{
	/*
	Connection conn;
	char sql[1024] = { 0 };
	sprintf(sql, "insert into sn(id,name,age,sex) values('%s','%s','%d','%s');", "1", "zhangsan", 20, "male");
	cout << sql << endl;
	conn.connect("127.0.0.1", 3306, "root", "123456", "chat");
	conn.update(sql);
	return 0;
	*/
	clock_t begin = clock();
	ConnectionPool* p = ConnectionPool::getConnectionPool();
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
	clock_t end = clock();
	cout << end - begin << "ms" << endl;
	return 0;
}