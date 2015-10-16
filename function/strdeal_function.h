#ifndef _STRDEAL_FUNCTION_H
#define _STRDEAL_FUNCTION_H

#include "../global_set.h"
#include "comdeal_function.h"

//给子进程的信息
typedef struct _deal_pthread_param
{
	my_string_list *string_list;
	struct bufferevent *bev;
}deal_pthread_param;

//用于命令传输
typedef struct _common_command_transfer
{
	int command_length;	//命令的长度
	unsigned char *command;	//编码之后的命令
}common_command_transfer;

typedef struct _pthread_global_param{
	//存储已经读取的命令
	my_string_list *command_tmp_list;
	//当前正在存储的命令类型
	unsigned char current_command_type;
	//预知的当前命令(有效字符串命令区域)的长度
	int command_length;
	//当前正在处理的字符的位置
	int now_byte_location;
	//当前调用的bufferevent
	struct bufferevent *bev;
}pthread_global_param;

//从socket中接收并处理链表
void deal_received_list(deal_pthread_param *wait_to_transfer);
//遍历查找每一个节点
void visit_each_node_char(unsigned char true_char,void *argc);
//处理校验好的字符串命令
void deal_command_string(char *command_string,pthread_global_param *the_pthread_global);
//检测、处理字符并保存字符到命令链表
void save_to_command_tmp_list(unsigned char true_char,pthread_global_param *the_pthread_global);
//给配置命令编码
common_command_transfer *set_command_encode(char *xml_string);
//释放生成的命令结构体
void free_command_encode(common_command_transfer * command_struct);
//获取客户端的ip地址
void get_client_ip(unsigned char *client_ip,struct bufferevent *bev);
//释放通用命令传输结构体
void free_command_encode(common_command_transfer * command_struct);
//初始化在子进程中使用到的全局变量
pthread_global_param *init_pthread_global_param();
//释放全局变量
void free_pthread_global_param(pthread_global_param *wait_free);

#endif