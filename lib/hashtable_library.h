#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

//每个节点
typedef struct _hash_node
{
    char *key;
    struct bufferevent *bev;
    struct bufferevent *control_bev;
    struct _hash_node *next;
}hash_node;

//初始化哈希表
void init_hash_table();
//哈希算法,获取位置
unsigned int hash(char *key);
//根据key的值来进行哈希查找
hash_node* get_hash_node(char *key);
//在内存中分配确定的地址来存储字符串
char* my_hash_table_malloc(char *ready_to_store);
//直接获取对应节点的事件
struct bufferevent* get_bufferevent(char *key);
//新增hash_node
int hash_node_add(char* key,struct bufferevent *bev);
//添加控制buffer
int hash_node_control_add(char* key,struct bufferevent *control_bev);
 
/* A pretty useless but good debugging function,
which simply displays the hashtable in (key.value) pairs
*/
void displaytable();

//清除整个表
void free_hash_table();