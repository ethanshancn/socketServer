#ifndef _GLOBAL_SET_H
#define _GLOBAL_SET_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

/*socket_function.h*/
#include <pthread.h>
#include <event2/event.h>
#include <event2/bufferevent.h>

#include "lib/strdeal_library.h"
#include "lib/xmldeal_library.h"
#include "lib/mysqldeal_library.h"
#include "lib/hashtable_library.h"

/*socket_function*/
#define MAX_RECEIVE_LENGTH    1040
#define LISTEN_PORT 8087

/*mysql*/
#define MYSQL_HOST "localhost"
#define MYSQL_USERNAME "root"
#define MYSQL_USERPASS "123456"
#define MYSQL_DBNAME "test"

/*全局变量*/
MYSQL *mysql_handle;



#endif