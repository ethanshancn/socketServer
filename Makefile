#生成的可执行文件的名称
TARGET = start_server

#各文件的路径
LIB_DIR = ./lib/
FUN_DIR = ./function/

SOURCES = $(wildcard *.c $(LIB_DIR)*.c $(FUN_DIR)*.c)

OBJS := $(patsubst %.c,%.o, $(SOURCES))

#添加libevent库的支持
CFLAGS = -L/usr/local/lib
CFLAGS += -levent

# 添加libxml2库的支持
CFLAGS += `xml2-config --cflags`
CFLAGS += `xml2-config --libs`

#增加GDB调试功能
CFLAGS += -g

#添加mysql的支持
CFLAGS += -L/usr/lib/i386-linux-gnu
CFLAGS += -lmysqlclient

all:$(TARGET)

%.d:%.c
	@set -e; rm -f $@; \
	$(CC) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

sinclude $(SOURCES:.c=.d)

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS);\
	rm -f $(SOURCES:.c=.d)

.PHONY:clean
clean:
	rm -f  $(TARGET) $(SOURCES:.c=.d) $(SOURCES:.c=.o)
