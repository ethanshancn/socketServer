#ifndef _MYSQLDEAL_LIBRARY_H
#define _MYSQLDEAL_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <mysql/mysql.h>

//根据宏定义建立数据库链接
MYSQL *new_mysql_connect(const char *host,const char *user_name,const char *user_pass,const char *db_name);
//释放MYSQL链接
void free_mysql_connect(MYSQL *my_mysql_connect);
//获取查询结果集
MYSQL_RES * my_mysql_query(MYSQL *my_mysql_connect,const char *query_char);
//释放查询结果集
void free_mysql_query_result(MYSQL_RES *result_all);
//执行INSERT、UPDATE、DELETE语句并返回影响的行数
unsigned long my_mysql_exec(MYSQL *my_mysql_connect,const char *exec_char);


#endif