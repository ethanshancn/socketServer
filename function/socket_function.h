#ifndef _SOCKET_FUNCTION_H
#define _SOCKET_FUNCTION_H

#include "../global_set.h"
#include "strdeal_function.h"

int start_socket_server ();

void do_accept(evutil_socket_t listener, short event, void *arg);

void read_cb(struct bufferevent *bev, void *arg);

void error_cb(struct bufferevent *bev, short event, void *arg);

void write_cb(struct bufferevent *bev, void *arg);

#endif