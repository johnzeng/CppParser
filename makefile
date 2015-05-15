mylib_PATH=../mylib
myLib=$(mylib_PATH)/mylib.a
PLATFORM_FLAG=-D_LINUX_
CC=clang
CPPCC=clang++
AR=ar
INCLUDE_FLAGS=-I./header -I$(mylib_PATH)/header
OTHER_C_FLAGS=-DDEBUG $(PLATFORM_FLAG)
OTHER_CPP_FLAGS=-DDEBUG $(PLATFORM_FLAG)
TARGET=precomile

%.o: %.c
	$(CC) $(OTHER_C_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@
%.o: %.cpp
	$(CPPCC) $(OTHER_CPP_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

SOURCES=$(wildcard ./*/*.c ./*/*.cpp)
OBJS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$TARGET:$(OBJS) $(myLib)
	$(CPPCC) -o $(TARGET) $(INCLUDE_FLAGS) $(OTHER_CPP_FLAGS) $(OBJS) $(myLib) main.cpp

$(myLib):
	cd $(mylib_PATH) && make
