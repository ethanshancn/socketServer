#ifndef _STRDEAL_LIBRARY_H
#define _STRDEAL_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct node
{
	unsigned  char true_string;	//暂时只填充一个字符
	struct node *last;
	struct node *next;

}my_string_node;

typedef struct node_list
{	
	my_string_node *head;
	my_string_node *tail;
	int size;
}my_string_list;

//初始化一个节点
my_string_node* init_my_string_node(unsigned char to_init_string);
//字符串初始化
my_string_list* init_my_string_list();
//在链表末尾添加字符串
void add_string(my_string_list *string_list,unsigned char to_add_string);
//获取第一个节点的内容
my_string_node *get_head(my_string_list *string_list);
//获取最后一个节点的内容
my_string_node *get_tail(my_string_list *string_list);
//获取下一个节点的内容
my_string_node *get_next_node(my_string_node *now_node);
//获取上一个节点的内容
my_string_node *get_last_node(my_string_node *now_node);
//获取节点中实际内容
unsigned char get_true_content(my_string_node *now_node);
//获取链表总的大小
int get_list_size(my_string_list *string_list);
//在now_node之前插入新节点
void insert_before_node(my_string_node *wait_to_insert_node,my_string_node *now_node,my_string_list *string_list);
//在now_node之后插入新节点
void insert_after_node(my_string_node *wait_to_insert_node,my_string_node *now_node,my_string_list *string_list);
//删除第一个节点
void free_first_node(my_string_list *string_list);
//删除最后一个节点
void free_end_node(my_string_list *string_list);
//删除某一个特定地址的节点
void free_certain_node(my_string_list *string_list,my_string_node *now_node);
//获取某一个特定节点的值(从0开始计算)
my_string_node *get_certain_node(my_string_list *string_list,int i);
//判断链表是否为空
int is_empty(my_string_list *string_list);
//依次对链表中每个元素调用函数visit_node()
void foreach_node(my_string_list *string_list,void (*visit_node)(unsigned char,void *),void *argc);
//清空整个链表
void clear_string_list(my_string_list *string_list);
//销毁整个链表
void free_string_list(my_string_list *string_list);
//销毁某一个节点
void free_node(my_string_node *string_node);
//分配空间并返回该字符串指针
char *my_list_to_char_malloc(my_string_list *string_list);
//转换链表至字符串变量
void list_to_char(my_string_list *string_list,char *char_string);

#endif