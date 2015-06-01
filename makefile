mylib_PATH=../mylib
myLib=$(mylib_PATH)/mylib.a
PLATFORM_FLAG=-D_LINUX_
CC=clang
CPPCC=clang++
AR=ar

debug_var=1
ifeq ($(debug_var),1)
RELEASE_FLAG=-DDEBUG -g
else
DEBUG_FLAG=-DRELEASE
endif

INCLUDE_FLAGS=-I./header -I$(mylib_PATH)/header
OTHER_C_FLAGS=
OTHER_CPP_FLAGS= 
OTHER_FLAGS=${RELEASE_FLAG} $(PLATFORM_FLAG)

TARGET=preprocessor

%.o: %.c makefile
	$(CC) $(OTHER_FLAGS) $(OTHER_C_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@
%.o: %.cpp makefile
	$(CPPCC) $(OTHER_FLAGS) $(OTHER_CPP_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

SOURCES=$(wildcard ./*/*.c ./*/*.cpp)
OBJS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$TARGET:$(OBJS) $(myLib) makefile
	$(CPPCC) -o $(TARGET) $(INCLUDE_FLAGS) ${OTHER_FLAGS} $(OTHER_CPP_FLAGS) $(OBJS) $(myLib) main.cpp

$(myLib):
	cd $(mylib_PATH) && make

lib:
	cd $(mylib_PATH) && make
release:clean makefile
	make debug_var=0	

clean:
	rm $(TARGET)
	rm $(OBJS)
