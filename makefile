mylib_PATH=../mylib
depend_generator=$(mylib_PATH)/depend_generator.sh
myLib=$(mylib_PATH)/mylib.a
PLATFORM_FLAG=-D_LINUX_
CC=clang
CXX=clang++
AR=ar

debug_var=1

ifeq ($(debug_var),1)
RELEASE_FLAG=-DDEBUG -g
else
RELEASE_FLAG=-DRELEASE
endif

INCLUDE_FLAGS=-I./header -I$(mylib_PATH)/header
OTHER_C_FLAGS=
OTHER_CPP_FLAGS=--std=c++0x
OTHER_FLAGS=${RELEASE_FLAG} $(PLATFORM_FLAG)

CFLAGS=$(INCLUDE_FLAGS) $(OTHER_FLAGS) $(OTHER_FLAGS)
CPPFLAGS=$(INCLUDE_FLAGS) $(OTHER_CPP_FLAGS) $(OTHER_FLAGS)
TEST_FLAG=-isystem $(mylib_PATH)/test/include $(mylib_PATH)/test/libtest.a

TARGET=target/libAnalyzer.a

SOURCES=$(wildcard ./src/*.cpp ./src/*/*.c ./src/*/*.cpp)
OBJS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(SOURCES)))
HEADERS=$(wildcard ./*/*.h)
TEST_SOURCE=$(wildcard ./test/src/*.cpp ./test/src/*/*.c ./test/src/*/*.cpp)

$(TARGET):$(OBJS) $(myLib) depend
	@echo $(OBJS)
	$(AR) -r $(TARGET) $(OBJS) $(myLib)

$(myLib):
	cd $(mylib_PATH) && make

test:$(TARGET) $(TEST_SOURCE)
	$(CXX) $(TARGET) $(TEST_SOURCE) $(TEST_FLAG) -o ./target/tester
	./target/tester

lib:
	cd $(mylib_PATH) && make

release:clean makefile
	make debug_var=0

.PHONY:count,clean,test,macro

clean:
	-rm depend
	-rm $(TARGET)
	-rm $(OBJS)

-include depend

count:
	wc -l $(HEADERS) $(SOURCES)


depend:$(HEADERS) $(SOURCES)
	@echo "=================== now gen depend =============="
	-rm depend
	-@sh $(depend_generator) "$(CPPFLAGS)" 2>&1 > /dev/null
