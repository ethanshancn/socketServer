#include "socket_function.h"

int start_socket_server ()
{
	evutil_socket_t listener;
	listener = socket(AF_INET, SOCK_STREAM, 0);
	assert(listener > 0);
	evutil_make_listen_socket_reuseable(listener);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(LISTEN_PORT);

	if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	    perror("bind");
	    return 1;
	}
	
	if (listen(listener, 32) < 0) {
	    perror("listen");
	    return 1;
	}

	printf ("Listening...\n");

	evutil_make_socket_nonblocking(listener);

	//设置启用多线程支持
	int evthread_use_pthreads(void);
	struct event_base *base = event_base_new();
	assert(base != NULL);
	struct event *listen_event;
	listen_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
	event_add(listen_event, NULL);
	event_base_dispatch(base);

	printf("The End.");
	return 0;
}

void do_accept(evutil_socket_t listener, short event, void *arg)
{
	struct event_base *base = (struct event_base *)arg;
	evutil_socket_t fd;
	struct sockaddr_in sin;
	socklen_t slen = sizeof(sin);
	fd = accept(listener, (struct sockaddr *)&sin, &slen);
	if (fd < 0) {
	    perror("accept");
	    return;
	}

	printf("ACCEPT: fd = %u\n", fd);

	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_cb, write_cb, error_cb, arg);
	bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);

	// //获取DCS基本信息
	char *get_dcs_inf = "get#<Number></Number><Location></Location><Customer></Customer><MaintenanceUnit></MaintenanceUnit><Tenement></Tenement><TenementCall></TenementCall>";
	common_command_transfer *wait_to_send = set_command_encode(get_dcs_inf);
	bufferevent_write(bev, wait_to_send->command, wait_to_send->command_length);
	free_command_encode(wait_to_send);

}

void read_cb(struct bufferevent *bev, void *arg)
{
	char line[MAX_RECEIVE_LENGTH+1];
	int n;
	evutil_socket_t fd = bufferevent_getfd(bev);

	my_string_list *new_string_list = init_my_string_list();

	while (n = bufferevent_read(bev, line, MAX_RECEIVE_LENGTH), n > 0) {

		printf("fd=%u, the true length is :%d\n", fd, n);

		int nl = 0;
		while(nl <n)
		{
			add_string(new_string_list,(unsigned char) line[nl]);
			nl ++;
		}
		bufferevent_write(bev, line, n);
	}

	//建立子线程，并将new_string_list，bev作为参数传入子线程中进行操作（因为参数只能传一个，故此用结构体指针来传递）
	pthread_t deal_p_id;

	deal_pthread_param *wait_to_transfer = (deal_pthread_param *)malloc(sizeof(deal_pthread_param));
	wait_to_transfer->string_list = new_string_list;
	wait_to_transfer->bev = bev;

	//设置线程自动释放
	pthread_attr_t attr;	//线程属性
	pthread_attr_init(&attr);	//初始化线程属性
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	//设置线程属性

	int ret=pthread_create(&deal_p_id, &attr, (void *) deal_received_list, wait_to_transfer);

	if(ret!=0)  
    {  
        printf ("Create pthread error!\n"); 
    } 
}

void write_cb(struct bufferevent *bev, void *arg)
{
	// printf("已写入数据\n");
}

void error_cb(struct bufferevent *bev, short event, void *arg)
{
	evutil_socket_t fd = bufferevent_getfd(bev);
	printf("fd = %u, ", fd);
	if (event & BEV_EVENT_TIMEOUT) {
	    printf("Timed out\n"); //if bufferevent_set_timeouts() called
	}
	else if (event & BEV_EVENT_EOF) {
	    printf("connection closed\n");
	}
	else if (event & BEV_EVENT_ERROR) {
	    printf("some other error\n");
	}
	bufferevent_free(bev);
}