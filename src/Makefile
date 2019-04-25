
CC = g++
LD = g++
SRCS = alldata.cpp  query.cpp  tinyxml2.cpp  chacha2.cpp  mohu.cpp  stdafx.cpp  utf.cpp  logger.cpp
OBJS = $(patsubst %cpp, %o, $(SRCS))

INCLUDE = -I.
LIB = -lfcgi

# 开启编译warning和设置优化等级
CFLAGS = -Wall -O2

TARGET = chacha3

.PHONY:all clean

all: $(TARGET)
# 链接时候指定库文件目录及库文件名
$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(LIB)
 
# 编译时候指定头文件目录
%o:%cpp
	$(CC) -c $^ $(INCLUDE) $(CFLAGS) 

clean:
	rm -f $(OBJS) $(TARGET)
