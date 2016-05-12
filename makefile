mylib_PATH=./mylib
depend_generator=$(mylib_PATH)/depend_generator.sh
myLib=$(mylib_PATH)/mylib.a
PLATFORM_FLAG=-D_LINUX_
#CC=clang
#CXX=clang++
AR=ar
GTEST_CHECKOUT=git clone https://github.com/google/googletest.git

debug_var=1

ifeq ($(debug_var),1)
RELEASE_FLAG=-DDEBUG -g -fno-pie -fprofile-arcs -ftest-coverage
MYLIB_CHECKOUT=git clone https://github.com/johnzeng/mylib.git -b develop
else 

ifeq($(debug_var), 2)
RELEASE_FLAG=-DDEBUG -g -fprofile-arcs -ftest-coverage
MYLIB_CHECKOUT=git clone https://github.com/johnzeng/mylib.git -b develop
endif

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
TEST_LIB=target/libtest.a

SOURCES=$(shell find ./src -type f -name '*.cpp') $(shell find ./src -type f -name '*.c')

OBJS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(SOURCES)))
HEADERS=$(wildcard ./*/*.h)
TEST_SOURCE=$(wildcard ./test/src/*.cpp ./test/src/*/*.c ./test/src/*/*.cpp)
TEST_OBJECTS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(TEST_SOURCE)))
TARGET_DIR=target

$(TARGET):$(OBJS) depend $(myLib) $(TARGET_DIR)
	@echo "=========================  now build target ================================"
	@echo $(OBJS)
	$(AR) -r $(TARGET) $(OBJS) 

	
$(myLib): $(mylib_PATH)
	@echo "=========================  now build mylib  ================================"
	cd $(mylib_PATH) && make

$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(TEST_LIB): $(TARGET_DIR)
	@echo " ======================= now build gtest =================================="
	$(GTEST_CHECKOUT)
	mkdir googletest/googletest/mybuild
	cd googletest/googletest/mybuild && cmake	../ && make && cd -
	cp googletest/googletest/mybuild/libgtest.a target/libtest.a
	ls target

$(mylib_PATH):
	@echo "now checkout mylib"
	$(MYLIB_CHECKOUT)

$(TEST_TARGET):$(TARGET) $(TEST_OBJECTS) $(TEST_LIB)
	@echo "=======================  build tester   ======================================="
	$(CXX) $(TARGET) $(TEST_OBJECTS) $(TEST_FLAG) $(TEST_LIB) $(CPPFLAGS) $(myLib) -o $(TEST_TARGET)

test:$(TEST_TARGET) $(SOURCES) $(HEADERS)
	@echo "==================== tester is going to run  ================================="
	-find . -name "*.gcda" -exec rm {} \;
	export C_INCLUDE_PATH="./" &&	export CPLUS_INCLUDE_PATH="./" && export OBJC_INC_ENV_PATH="./" && ./$(TEST_TARGET)

lib:
	cd $(mylib_PATH) && make

release:clean makefile
	make debug_var=0

travis:
	make test debug_var=2

debuger:$(TEST_TARGET)
	lldb $(TEST_TARGET)

clean:
	-find . -name "*.gcno" -exec rm {} \;
	-find . -name "*.gcda" -exec rm {} \;
	-rm depend
	-rm $(OBJS)
	-rm -rf ./target/*
	-rm *.html

-include depend

count:
	wc -l $(HEADERS) $(SOURCES) $(TEST_SOURCE)

cov:
	@echo "=================== now gen cov html file ============================="
	gcovr -r . -e mylib --html --html-details -o target/cov.html


depend:$(HEADERS) $(SOURCES) $(mylib_PATH)
	@echo "=================== now gen depend  =================================="
	-@sh $(depend_generator) "$(CPPFLAGS) $(TEST_FLAG)" 2>&1 > /dev/null

.PHONY:count,clean,test,macro,debuger,release,cov
