#include "mysqldeal_library.h"

//根据宏定义建立数据库链接
MYSQL *new_mysql_connect(const char *host,const char *user_name,const char *user_pass,const char *db_name)
{
	MYSQL *my_mysql_connect;
	my_mysql_connect = mysql_init(NULL);
	if (!mysql_real_connect(my_mysql_connect,host,user_name,user_pass,db_name,0,NULL,0))
	{
		printf("Couldn't connect to engine!\n%s\n",mysql_error(my_mysql_connect) );
		exit(1);
	}
	return my_mysql_connect;
}

//释放MYSQL链接
void free_mysql_connect(MYSQL *my_mysql_connect)
{
	mysql_close(my_mysql_connect);
}

//获取查询结果
MYSQL_RES * my_mysql_query(MYSQL *my_mysql_connect,const char *query_char)
{
	MYSQL_RES *result_all;
	// MYSQL_ROW each_row;

	if (mysql_query(my_mysql_connect,query_char))
	{
		printf("Query failed (%s)\n",mysql_error(my_mysql_connect) );
		exit(1);
	}

	if (!(result_all = mysql_store_result(my_mysql_connect)))
	{
		printf("Couldn't get result from %s\n", mysql_error(my_mysql_connect));
		exit(1);
	}

	return result_all;

}

//释放查询结果集
void free_mysql_query_result(MYSQL_RES *result_all)
{
	mysql_free_result(result_all);
}

//执行INSERT、UPDATE、DELETE语句并返回影响的行数
unsigned long my_mysql_exec(MYSQL *my_mysql_connect,const char *exec_char)
{
	if (mysql_query(my_mysql_connect,exec_char))
	{
		printf("Exec failed (%s)\n",mysql_error(my_mysql_connect) );
		exit(1);
	}

	unsigned long affected_rows = (unsigned long)mysql_affected_rows(my_mysql_connect);

	return affected_rows;
}