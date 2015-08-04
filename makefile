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

TARGET=analyzer

SOURCES=$(wildcard ./*/*.c ./*/*.cpp)
OBJS=$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(SOURCES)))
HEADERS=$(wildcard ./*/*.h)

$(TARGET):$(OBJS) $(myLib) main.o depend
	@echo $(OBJS)
	$(CXX) -o $(TARGET) $(INCLUDE_FLAGS) ${OTHER_FLAGS} $(OTHER_CPP_FLAGS) $(OBJS) $(myLib) main.o

$(myLib):
	cd $(mylib_PATH) && make

test:$(TARGET)
	-@rm log
	./$(TARGET) testcpp

testm:$(TARGET)
	./$(TARGET) TestSet/macro_test_1|grep top
	

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
