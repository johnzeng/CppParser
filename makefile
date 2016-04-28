mylib_PATH=./mylib
depend_generator=$(mylib_PATH)/depend_generator.sh
myLib=$(mylib_PATH)/mylib.a
PLATFORM_FLAG=-D_LINUX_
CC=clang
CXX=clang++
AR=ar

debug_var=1

ifeq ($(debug_var),1)
RELEASE_FLAG=-DDEBUG -g -fno-pie -fprofile-arcs -ftest-coverage
MYLIB_CHECKOUT=git clone https://github.com/johnzeng/mylib.git -b develop
else
RELEASE_FLAG=-DRELEASE
MYLIB_CHECKOUT=git clone https://github.com/johnzeng/mylib.git -b master
endif

INCLUDE_FLAGS=-I./header -I$(mylib_PATH)/header
OTHER_C_FLAGS=
OTHER_CPP_FLAGS=--std=c++0x
OTHER_FLAGS=${RELEASE_FLAG} $(PLATFORM_FLAG)

TEST_FLAG=-isystem $(mylib_PATH)/test/include 
CFLAGS=$(INCLUDE_FLAGS) $(OTHER_FLAGS) $(OTHER_FLAGS) $(TEST_FLAG)
CPPFLAGS=$(INCLUDE_FLAGS) $(OTHER_CPP_FLAGS) $(OTHER_FLAGS) $(TEST_FLAG)

TARGET=target/libAnalyzer.a
TEST_TARGET=target/tester

SOURCES=$(wildcard ./src/*.cpp ./src/*/*.c ./src/*/*.cpp)
OBJS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(SOURCES)))
HEADERS=$(wildcard ./*/*.h)
TEST_SOURCE=$(wildcard ./test/src/*.cpp ./test/src/*/*.c ./test/src/*/*.cpp)
TEST_OBJECTS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(TEST_SOURCE)))

$(TARGET):$(OBJS) depend $(myLib)
	@echo $(OBJS)
	$(AR) -r $(TARGET) $(OBJS) 

	
$(myLib): $(mylib_PATH)
	@echo "now build mylib"
	cd $(mylib_PATH) && make

$(mylib_PATH):
	@echo "now checkout mylib"
	$(MYLIB_CHECKOUT)

$(TEST_TARGET):$(TARGET) $(TEST_OBJECTS)
	@echo "==================== build tester ==========================="
	$(CXX) $(TARGET) $(TEST_OBJECTS) $(TEST_FLAG) $(mylib_PATH)/test/libtest.a $(CPPFLAGS) $(myLib) -o $(TEST_TARGET)

test:$(TEST_TARGET) $(SOURCES) $(HEADERS)
	-find . -name "*.gcda" -exec rm {} \;
	@echo "==================== tester build finished ==========================="
	export C_INCLUDE_PATH="./" &&	export CPLUS_INCLUDE_PATH="./" && export OBJC_INC_ENV_PATH="./" && ./$(TEST_TARGET)
	gcovr -r . -e mylib

lib:
	cd $(mylib_PATH) && make

release:clean makefile
	make debug_var=0

debuger:$(TEST_TARGET)
	lldb $(TEST_TARGET)

clean:
	-find . -name "*.gcno" -exec rm {} \;
	-rm depend
	-rm $(OBJS)
	-rm -rf ./target/*

-include depend

count:
	wc -l $(HEADERS) $(SOURCES)

cov:
	gcovr -r . -e mylib --html --html-details -o cov.html


depend:$(HEADERS) $(SOURCES) $(mylib_PATH)
	@echo "=================== now gen depend =============="
	- mkdir target
	-@sh $(depend_generator) "$(CPPFLAGS) $(TEST_FLAG)" 2>&1 > /dev/null

.PHONY:count,clean,test,macro,debuger,release,cov
