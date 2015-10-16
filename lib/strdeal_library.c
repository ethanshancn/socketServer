#include "strdeal_library.h"

//初始化一个节点
my_string_node* init_my_string_node(unsigned char to_init_string)
{
	my_string_node *return_node = (my_string_node *)malloc(sizeof(my_string_node));
	if (return_node != NULL)
	{	
		return_node->true_string = to_init_string;
		return_node->last = NULL;
		return_node->next = NULL;
	}
	else
	{
		return NULL;
	}
	return return_node;
}

//字符串初始化
my_string_list* init_my_string_list()
{
	my_string_list *return_list = (my_string_list *)malloc(sizeof(my_string_list));
	if (return_list != NULL)
	{
		return_list->head = NULL;
		return_list->tail = NULL;
		return_list->size = 0;
	}
	else
	{
		return NULL;
	}
	return return_list;
}

//在链表末尾添加字符串
void add_string(my_string_list *string_list,unsigned char to_add_string)
{
	my_string_node *now_end_node = get_tail(string_list);
	my_string_node *new_node = init_my_string_node(to_add_string);
	if (new_node != NULL)
	{
		if (now_end_node != NULL)
		{
			now_end_node->next = new_node;
			new_node->last = now_end_node;
			string_list->tail = new_node;
			string_list->size ++;
		}
		else
		{
			string_list->head = new_node;
			string_list->tail = new_node;
			string_list->size ++;
		}
	}
}

//获取第一个节点的内容
my_string_node *get_head(my_string_list *string_list)
{
	return string_list->head;
}

//获取最后一个节点的内容
my_string_node *get_tail(my_string_list *string_list)
{
	return string_list->tail;
}

//获取下一个节点的内容
my_string_node *get_next_node(my_string_node *now_node)
{
	return now_node->next;
}

//获取上一个节点的内容
my_string_node *get_last_node(my_string_node *now_node)
{
	return now_node->last;
}

//获取节点中实际内容
unsigned char get_true_content(my_string_node *now_node)
{
	return now_node->true_string;
}

//获取链表总的大小
int get_list_size(my_string_list *string_list)
{
	return string_list->size;
}

//在now_node之前插入新节点
void insert_before_node(my_string_node *wait_to_insert_node,my_string_node *now_node,my_string_list *string_list)
{
	if (now_node->last != NULL)
	{
		now_node->last->next = wait_to_insert_node;
		wait_to_insert_node->last = now_node->last;
		wait_to_insert_node->next = now_node;
		now_node->last = wait_to_insert_node;
	}
	else 	//判断为字符链表的首地址
	{
		string_list->head = wait_to_insert_node;
		wait_to_insert_node->next = now_node;
		now_node->last = wait_to_insert_node;
	}
	string_list->size++;
}

//在now_node之后插入新节点
void insert_after_node(my_string_node *wait_to_insert_node,my_string_node *now_node,my_string_list *string_list)
{
	if (now_node->next != NULL)
	{
		now_node->next->last = wait_to_insert_node;
		wait_to_insert_node->next = now_node->next;
		wait_to_insert_node->last = now_node;
		now_node->next = wait_to_insert_node;
	}
	else	//判断为字符链表的尾地址
	{
		string_list->tail = wait_to_insert_node;
		wait_to_insert_node->last = now_node;
		now_node->next = wait_to_insert_node;
	}
	string_list->size++;
}

//删除第一个节点
void free_first_node(my_string_list *string_list)
{
	my_string_node *ready_to_free = get_head(string_list);
	if (ready_to_free != NULL)
	{
		if (ready_to_free->next != NULL)
		{
			string_list->head = ready_to_free->next;
			ready_to_free->next->last = NULL;		
		}
		else
		{
			string_list->head = NULL;
			string_list->tail = NULL;
		}
		string_list->size --;
		free_node(ready_to_free);
	}
}

//删除最后一个节点
void free_end_node(my_string_list *string_list)
{
	my_string_node *ready_to_free = get_tail(string_list);
	if (ready_to_free != NULL)
	{
		if (ready_to_free->last != NULL)
		{
			string_list->tail = ready_to_free->last;
			ready_to_free->last->next = NULL;
		}
		else
		{
			string_list->head = NULL;
			string_list->tail = NULL;
		}
		string_list->size --;
		free_node(ready_to_free);
	}
}

//删除某一个特定地址的节点
void free_certain_node(my_string_list *string_list,my_string_node *now_node)
{
	if (now_node->last == NULL)
	{
		free_first_node(string_list);
	}
	else if(now_node->next == NULL)
	{
		free_end_node(string_list);
	}
	else
	{
		now_node->last->next = now_node->next;
		now_node->next->last = now_node->last;
		string_list->size --;
		free_node(now_node);
	}
}

//获取某一个特定节点的值(从0开始计算)
my_string_node *get_certain_node(my_string_list *string_list,int i)
{
	my_string_node *return_node = get_head(string_list);
	int j = 0 ;
	while ( j < i && return_node != NULL)
	{
		return_node = return_node->next;
		j++;
	}
	return return_node;
}

//判断链表是否为空
int is_empty(my_string_list *string_list)
{
	if (get_list_size(string_list) == 0 && (get_head(string_list) == get_tail(string_list)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//依次对链表中每个元素调用函数visit_node()
void foreach_node(my_string_list *string_list,void (*visit_node)(unsigned char,void *),void *argc)
{
	my_string_node *now_node = get_head(string_list);
	if (is_empty(string_list))
	{
		exit(0);
	}
	else
	{
		while(now_node->next != NULL)
		{
			visit_node(now_node->true_string,argc);
			now_node = now_node->next;
		}
		visit_node(string_list->tail->true_string,argc);
	}
}

//清空整个链表
void clear_string_list(my_string_list *string_list)
{
	while(get_list_size(string_list) > 0)
	{
		free_end_node(string_list);
	}

}

//销毁整个链表
void free_string_list(my_string_list *string_list)
{	
	if (is_empty(string_list) == 0)
	{
		clear_string_list(string_list);
	}
	free(string_list);
}

//销毁某一个节点
void free_node(my_string_node *string_node)
{
	free(string_node);
}

//分配空间并返回该字符串指针
char *my_list_to_char_malloc(my_string_list *string_list)
{
	int char_length = get_list_size(string_list)+1;
	char *wait_return = (char *)malloc(char_length*sizeof(char));
	return wait_return;
}

//转换链表至字符串变量
void list_to_char(my_string_list *string_list,char *char_string)
{
	my_string_node *now_node = get_head(string_list);
	int i = 0 ;
	int list_length = get_list_size(string_list);
	while ( i < list_length && now_node != NULL)
	{
		char_string[i] = now_node->true_string;
		now_node = now_node->next;
		i++;
	}
	char_string[list_length] = '\0';
}