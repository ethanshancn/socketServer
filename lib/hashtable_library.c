#include "hashtable_library.h"

#define TABLE_SIZE 10240

static hash_node* HASH_TABLE[TABLE_SIZE];

//初始化哈希表
void init_hash_table()
{
    unsigned int i = 0;
    while( i < TABLE_SIZE )
    {
        HASH_TABLE[i] = NULL;
        i++;
    }
}

//哈希算法,获取位置
unsigned int hash(char *key)
{
	unsigned int h=0;
	while(*key)
	{
		h = h * 5831 + *key;
		key ++ ;
	}
	return h%TABLE_SIZE;
}

//根据key的值来进行哈希查找
hash_node* get_hash_node(char *key)
{
	unsigned int hi=hash(key);
	hash_node* np=HASH_TABLE[hi];
	while(np!=NULL)
	{
		if(!strcmp(np->key,key))
		{
			return np;
		}
		np=np->next;
	}
	return NULL;
}

//在内存中分配确定的地址来存储字符串
char* my_hash_table_malloc(char *ready_to_store)
{
    int l=strlen(ready_to_store)+1;
    char *ns=(char*)malloc(l*sizeof(char));
    strcpy(ns,ready_to_store);
    if(ns==NULL)
        return NULL;
    else
        return ns;
}

//直接获取对应节点的事件
struct bufferevent* get_bufferevent(char *key)
{
    hash_node* n=get_hash_node(key);
    if(n==NULL)
        return NULL;
    else
        return n->bev;
}

//新增hash_node
int hash_node_add(char* key,struct bufferevent *bev)
{
    unsigned int hi;
    hash_node* np;
    if((np=get_hash_node(key))==NULL)
    {
        hi=hash(key);
        np=(hash_node *)malloc(sizeof(hash_node));
        if(np==NULL)
            return 0;
        np->key=my_hash_table_malloc(key);
        if(np->key==NULL) return 0;
        np->next=HASH_TABLE[hi];
        HASH_TABLE[hi]=np;
    }
    else 
    {
        if (np->bev != NULL)
        {
            
        }
        if (np->control_bev != NULL)
        {
            bufferevent_free(np->control_bev);
        }
    }
    np->bev=bev;
    if(np->bev==NULL) return 0;
 
    return 1;
}

//添加控制buffer
int hash_node_control_add(char* key,struct bufferevent *control_bev)
{
    unsigned int hi;
    hash_node* np;
    if ((np = get_hash_node(key)) != NULL)
    {
        if (np->control_bev != NULL)
        {
            bufferevent_free(np->control_bev);
        }
        np->control_bev = control_bev;
    }
    else
    {
        return 0;
    }
    return 1;
}
 
/* A pretty useless but good debugging function,
which simply displays the hashtable in (key.value) pairs
*/
void displaytable()
{
    // int i;
    // hash_node *t;
    // for(i=0; i <TABLE_SIZE ; i++)
    // {
    //     if(HASH_TABLE[i]==NULL)
    //         printf("()");
    //     else
    //     {
    //         t=HASH_TABLE[i];
    //         printf("(");
    //         for(; t!=NULL; t=t->next)
    //             printf("(%s) ",t->key);
    //         printf(".)");
    //     }
    // }
}

//清除整个表
void free_hash_table()
{
    int i;
    hash_node *np,*t;
    for(i=0; i < TABLE_SIZE; i++)
    {
        if(HASH_TABLE[i]!=NULL)
        {
            np=HASH_TABLE[i];
            while(np!=NULL)
            {
                t=np->next;
                free(np->key);
                bufferevent_free(np->bev);
                free(np);
                np=t;
            }
        }
    }
}