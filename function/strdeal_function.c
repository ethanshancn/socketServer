#include "strdeal_function.h"

//从socket中接收并处理链表
void deal_received_list(deal_pthread_param *wait_to_transfer)
{
	//子进程全局变量
	pthread_global_param *the_pthread_global = (pthread_global_param *)init_pthread_global_param();
	the_pthread_global->bev = wait_to_transfer->bev;

	if (get_list_size(wait_to_transfer->string_list) > 0)
	{
		foreach_node(wait_to_transfer->string_list, visit_each_node_char ,the_pthread_global);
	}

	free_pthread_global_param(the_pthread_global);
	free_string_list(wait_to_transfer->string_list);
	free(wait_to_transfer);
}

//遍历每一个节点,并进行相应的处理
void visit_each_node_char(unsigned char true_char,void *argc)
{
	pthread_global_param *the_pthread_global = (pthread_global_param *)argc;

	// printf("正在处理的位置是：%d,正在处理的字符是:%x\n", the_pthread_global->now_byte_location,true_char);
	//Start receive.
	if ((true_char == 0x80 || true_char == 0xc0) && (the_pthread_global->command_tmp_list == NULL))
	{
		the_pthread_global->command_tmp_list = init_my_string_list();
	}

	//Save to list.
	if (the_pthread_global->command_tmp_list != NULL)
	{
		save_to_command_tmp_list(true_char,the_pthread_global);
	}

	//Received all of the command.
	if ((true_char == 0x81 || true_char == 0xc1) && (the_pthread_global->command_tmp_list != NULL))
	{
		if (the_pthread_global->command_length == get_list_size(the_pthread_global->command_tmp_list))
		{
			char *command_string = my_list_to_char_malloc(the_pthread_global->command_tmp_list);
			list_to_char(the_pthread_global->command_tmp_list,command_string);

			//To deal with command.
			deal_command_string(command_string,the_pthread_global);

			free(command_string);
		}
		free_string_list(the_pthread_global->command_tmp_list);

		the_pthread_global->command_tmp_list = NULL;
		the_pthread_global->current_command_type = 0xFF;
		the_pthread_global->now_byte_location = 0;
		the_pthread_global->command_length = 0;
	}

}

//检测、处理字符并保存字符到命令链表
void save_to_command_tmp_list(unsigned char true_char,pthread_global_param *the_pthread_global)
{
	if (the_pthread_global->now_byte_location == 1)
	{
		the_pthread_global->current_command_type = true_char;
	}

	//The heartbeat/configuration command.
	if (the_pthread_global->current_command_type == 0x04 || the_pthread_global->current_command_type == 0x10)
	{
		if (the_pthread_global->now_byte_location == 3)
		{
			the_pthread_global->command_length = (int)true_char * 256;
		}
		else if (the_pthread_global->now_byte_location == 4)
		{
			the_pthread_global->command_length += (int)true_char;
		}
		else if (the_pthread_global->now_byte_location > 4 && the_pthread_global->now_byte_location < (the_pthread_global->command_length + 5))
		{
			add_string(the_pthread_global->command_tmp_list,true_char);
		}
	}
	printf("正在处理的位置是：%d,正在处理的字符是:%x\n", the_pthread_global->now_byte_location,true_char);
	//判断为心跳命令
	// else if(the_pthread_global->current_command_type == 0x10)
	// {

		
	// }
	//The other command.
	// else if (the_pthread_global->current_command_type == 0xFF)
	// {}x
	the_pthread_global->now_byte_location ++;
}

//处理校验好的字符串命令
void deal_command_string(char *command_string,pthread_global_param *the_pthread_global)
{
	if (command_string != NULL)
	{
		printf("等待处理的字符串命令是：%s\n", command_string);
		//获取xml命令字符串
		char *head_mark;
		char *separate_mark = "#";

		head_mark = strtok(command_string,separate_mark);
		char *xml_string = strtok(NULL,separate_mark);

		if (head_mark != NULL) 
		{
			//判断为用户权限验证
			if (strcasecmp("init",head_mark) == 0)
			{
				printf("-------->文字相同\n");
			}
			//判断为其他设置信息
			else if(strcasecmp("get",head_mark) == 0 || strcasecmp("set",head_mark) == 0)
			{
				printf("XML命令是：%s\n",xml_string);

				//判断为DCS基本信息
				if (strstr(xml_string,"Number") != NULL && strstr(xml_string,"Location") != NULL && strstr(xml_string,"Customer") != NULL)
				{
					printf("判断为DCS基本信息\n");








				}
				//判断为实时信息
				else if (strstr(xml_string,"Theme") != NULL && strstr(xml_string,"Floor") != NULL && strstr(xml_string,"Arrow") != NULL && strstr(xml_string,"Warn") != NULL)
				{
					printf("判断为实时信息\n");
				}
			}
		}
		else
		{
			printf("head_mark是空\n" );
		}
	}
	
}

//回复心跳命令
common_command_transfer *heart_beat_command_encode(char *xml_string)
{
	int command_length = 7;
	unsigned char *after_command = (unsigned char *)malloc(sizeof(unsigned char)*command_length);
	after_command[0] = 0xc0;
	after_command[1] = 0x10;
	after_command[2] = 0x00;
	after_command[3] = 0x00;
	after_command[4] = 0x00;
	after_command[5] = 0x00;
	after_command[6] = 0xc0;

	common_command_transfer *ready_return = (common_command_transfer *)malloc(sizeof(common_command_transfer));
	ready_return->command_length = command_length;
	ready_return->command = after_command;
	return ready_return;
}

//给配置命令编码
common_command_transfer *set_command_encode(char *xml_string)
{
	int command_length = strlen(xml_string);

	unsigned char *after_command;
	after_command = (unsigned char *)malloc(sizeof(unsigned char)*(command_length+7));

	after_command[0] = 0x80;
	after_command[1] = 0x04;
	after_command[2] = 0x00;
	after_command[3] = (unsigned char)(command_length/256);
	after_command[4] = (unsigned char)(command_length%256);
	int i = 0;
	while(i < command_length)
	{
		i++;
		after_command[i+4] = (int)*xml_string;
		xml_string++;
	}
	after_command[i+5] = 0x00;
	after_command[i+6] = 0x81;
	
	common_command_transfer *ready_return = (common_command_transfer *)malloc(sizeof(common_command_transfer));
	ready_return->command_length = command_length+7;
	ready_return->command = after_command;
	return ready_return;
}

//释放通用命令传输结构体
void free_command_encode(common_command_transfer * command_struct)
{
	free(command_struct->command);
	free(command_struct);
}

//获取客户端的ip地址
void get_client_ip(unsigned char *client_ip,struct bufferevent *bev)
{
	if (bev != NULL)
	{
		evutil_socket_t fd = bufferevent_getfd(bev);
		struct sockaddr_in sa;
		int len = sizeof(sa);
		if(!getpeername(fd, (struct sockaddr *)&sa, &len))
		{
			strcpy(client_ip,(char *)inet_ntoa(sa.sin_addr));
		}
	}
}

//初始化在子进程中使用到的全局变量
pthread_global_param *init_pthread_global_param()
{
	pthread_global_param *ready_to_return = (pthread_global_param *)malloc(sizeof(pthread_global_param));
	//存储已经读取的命令
	ready_to_return->command_tmp_list = NULL;
	//当前正在存储的命令类型
	ready_to_return->current_command_type = 0xFF;
	//预知的当前命令(有效字符串命令区域)的长度
	ready_to_return->command_length = 0;
	//当前正在处理的字符的位置
	ready_to_return->now_byte_location = 0;

	ready_to_return->bev = NULL;

	return ready_to_return;
}

//释放全局变量
void free_pthread_global_param(pthread_global_param *wait_free)
{
	if (wait_free!=NULL)
	{
		if (wait_free->command_tmp_list != NULL)
		{
			free_string_list(wait_free->command_tmp_list);
		}
		free(wait_free);
	}
}