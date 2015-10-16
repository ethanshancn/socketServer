#include "global_set.h"
#include "function/socket_function.h"

//初始化数据库连接及xml
void init_library()
{
	//连接数据库
	mysql_handle = new_mysql_connect(MYSQL_HOST,MYSQL_USERNAME,MYSQL_USERPASS,MYSQL_DBNAME);
	//初始化xml解析器
	xmlInitParser();
	//初始化事件hash表
	init_hash_table();
}

int main()
{
	init_library();
	start_socket_server();
	return 0;
}