**日志将继续在有道云维护**
[devlog part1](http://note.youdao.com/yws/public/redirect/share?id=61ec3b3371f2d0852c799484a066d543&type=false)
[devlog part2](http://note.youdao.com/yws/public/redirect/share?id=85bd1c5e254d8545bf91ae3d027624ab&type=false)
#Develop Daily
This file only contain the daily log. Nothing useful

+_+
##2016/05/14
Log1:基本完整部署所有lex测试。发布新的版本**0.1.3**
##2016/05/13
Log1:修复一些g++编译问题，仍旧搞不明白为什么链接不上我的.a文件。
Log2:已经知道有部分bnf存在循环。如果直接跑可能会出现死循环的问题。需要考虑加入loop breaker，在调用函数之前压入行号，如果在同样的index上面回到同样的行号，就是loop了。

##2016/05/12
Log1:增加大量bnf代码。bnf现在有左迭代和右迭代。这里面是不是有区别呢。。。
Log2:增加travis的gtest。由于环境变量设置不正确，导致其中一个test不通过

##2016/05/11
Log1:增加funcDecl的部分代码。。。。那个bnf不要太长。。。。目前的计划是先把bnf实现了。后面的分析无非就是各种返回而已，比较好说。
##2016/05/10
Log1:enum类型相当于省略了一个int标签，想想他跟其他的类型好像是一样的。
Log2:增加Const volatile测试队列。增加一部分符号识别。
Log3:enum在c＋＋11标准下面增加了新的特性， 可以支持enum class, enum struct的定义了。或者是enum hello: int {} 的定义。支持定义枚举类型了。
Log4: c++11 特性，[attributes](http://en.cppreference.com/w/cpp/language/attributes)。目前看来这个特性不影响编译和解释。我认为应该先编写代码skip掉他们。

##2016/05/09
Log1:
interesting error:

```
src/Core/Grammar/GrammarData.cpp:124:42: error: expected '(' after 'dynamic_cast'
      block = dynamic_cast<GrammarBlock*>father;
                                         ^
                                         (
1 error generated.
```

##2016/05/06
Log1:BugFix, 可以识别llu,ull等后缀了，不能识别e常量这个有待斟酌。另外还有\u \U 开头的char问题
Log2:BugFix, llu,ull,增加e常量的识别。
##2016/05/05
Log1:数据结构仍旧不够清晰，无法很好地描述当前节点以及不好向下传递。

##2016/05/04
Log1: 词法器bug，不能识别常量的u尾缀。不能识别e开头的指数常量。
##2016/05/03
Log1: 神奇的返回函数指针的函数

```cpp
#include <stdio.h>

typedef void * hello();

int test (int a, int b)
{
  printf("hello %d", a+b);
  return a+b;
}

int (*getFunc()) (int,int) 
{
    return test;
}

int (*(*getGetFunc()) ())(int,int)
{
  return getFunc;
}

int main(int argc, const char *argv[]){
  getGetFunc()()(1,2);
  return 0;
}
```

`getFunc `返回一个`int(*)(int,int)`指针。似乎是迭代的。`getGetFunc`返回一个`int(*(*()))(int,int)` 这种签名不要太坑。。

Log2:注意类型定义里面的括号不能随意乱加，否则应该抛异常,但是，有些时候又是可以随意加的，例如int ( test ) (int a, int b)和int ( (*getFunc()) ) (int,int)都是可以的。。。难过。。。怎么定义呢。。。

##2016/04/29
Log1: 神奇的函数指针：
typedef void (*he)();
定义一个he类型，是指向void （）类型函数的指针。

```cpp
#include <stdio.h>
typedef void (* hello)();
void a(){
  printf("hello");
}
int main(int argc, const char *argv[]){
  hello ptr = a;
  ptr();
  return 0;
}
```

另一方面，
typedef void * he();
定义一个类型，这个类型定义的变量为void* ()函数变量。相当于定义这个函数名，函数的输入为空，输出为void*

```cpp
#include <stdio.h>
typedef void * hello();
hello a;
void* a(){
  printf("hello");
  return NULL;
}
int main(int argc, const char *argv[]){
  a();
  return 0;
}
```

注意这里定义的是函数名，跟函数指针是不同的东西
##2016/04/28
Log1: 增加了行数统计。目前还在增加测试用例保证各种情况得到的结果都是对的
Log2: 目前增加新的0.1.1tag，当前版本应该已经完成大部分预编译和词法分析需要做的东西了。接下来就是语法分析了。
Log3: 马上开始Grammar分析的设计，先使用傻瓜办法，进行基本的语法分析。

##2016/04/27
Log1:增加错误测试。有一个这样的问题，如果宏展开是一个循环定义的话，没有很好识别出来。目前看来这个问题是由于FileReaderRecordType定义的混乱导致的。
Log2:校验之后发现上面的bug不是bug，但是filetype已经修正。
Log3:目前可以查到的范围内的测试都加上了。
**Log4:0.1.0版本已经finish**
##2016/04/26
Log1:增加部分覆盖率测试。目前主要的覆盖率缺失在Lex.cpp以及MacroCheck
Log2:Lex.cpp覆盖率达成100， 貌似很多覆盖不到的都是错误处理。没发覆盖了。

##2016/04/21
Log1: 目前已经将所有的测试加上了。后面加上一个覆盖性测试就差不多了。
Log2: 貌似覆盖率测试还是挺简单的，就是报告声称优点麻烦，后面研究下怎么弄

##2016/04/20
Log1:目前来说比较直接的想法是在判断为无用流之后直接开一个函数读掉他们，在heartbeat那里做修改，如果是无用流，直接eatUntilNextSharp，并且不能eat掉sharp.
Log2:important:正式更名为CppParser

##2016/04/18
Log1: 多数跟函数展开的bug已经修复了。但是还有一个gtest的问题。因为gtest是多线程的，所以并不知道会不会有跨线程的bug。目前看来，测试用例似乎存在这种问题
Log2: 推翻Log1，似乎是解析是不是变长参数的时候出问题了。有一点随机在里面。估计又是buff之类的问题，明天再看了。
Log3: 推翻Log2，不是buff问题，是逻辑直接邮问题
Log4: 基本#define宏益精修复完大部分的bug了。现在开始流检测，需要将isLastStreamUseful这个东西提取出来，对于无用的流，直接过滤掉才行

##2016/04/17
Log1: 是时候对类的函数进行访问控制细分了。。。

##2016/04/15
Log1: 修复LexBase的bug，基本抽离完成，LexBase类抽离基本的单词读取，字符串识别等工作

##2016/04/14
Log1: 基本重构完成LexBase, 基本的行为都抽取出来了。但是现在心跳那里有bug。

##2016/04/12
Log1: 开始重构Lex，将提取出Base，进行抽象

##2016/04/11
Log1: 宏展开在输入有明显的问题。逻辑非常勉强，太多特殊处理了。不如重新写一个独立的处理函数进行处理。例如说，写一个叫做readMacroParams的函数进行读取
Log2: 单宏展开之后没有进行doLex。是不是存在这样的问题，如果是重叠定义，就没发展开了 [test case added, 是我多心了。。]
Log3: 目前最直接的办法是重构Lex结构，将会重构成LexBase, CppLex, MacroLex等几个不同的Lex

##2016/04/10
Log1: 宏的展开还是有问题，可能需要整个重构

2016/04/06
Log1: 增加了str转换为数字的接口。依旧一堆bug。。。我觉得还是使用std库好。我傻逼了，干嘛自己写
Log2: 请参考atof, atoi, atol, atoll几个函数进行转换

##2016/04/02
Log1: 增加一些语法测试语句，还是蛮多bug的 (笑着哭)

##2016/04/01
Log1: 浮点数处理的方法不是特别好，最好换一下
Log2: 增加一个map，标记抹去注释和分行符号的位置，这样就能准确获取到某个抹去后的位置在哪一行了

##2015/08/19
Log1:	浮点数常量处理存在问题。需要解决下

##2015/08/05
Log1:	预编译阶段基本完成。目前进入一段时间的测试期。

##2015/08/04
Log1:	关于#if宏的匹配问题，需要重新研究才行，各种地方都非常混乱，可能需要整个重写
2015/08/01
Log3:	目前完成了大部分预编译工作了，还要做一个宏布尔运算器，计算#if,#elif的值,最后一点工作了，剩下就是测试了

Log2:	注意，define的链接符##会比宏定义优先处理也就是说下面的定义：

```cpp
#define HELLO te
#define hell() HELLO ## st
```
则，hell()会展开为HELLOst，而不是test。

Log1:	__VA_ARGS__宏只在可变参数名没有给定的情况下可以使用。例如：

File: macro_test.cpp

```cpp
		#include <stdio.h>

		#define B(he...) printf(he);
		int main(int argc, const char *argv[])
		{
				B("hello");
		}
```

File: macro_test1.cpp

```cpp
		#include <stdio.h>

		#define B(...) printf(__VA_ARGS__);
		int main(int argc, const char *argv[])
		{
				B("hello");
		}
```

上述两个文件可以编译通过，都是输出hello（不带回车）。但是下面这个文件无法编译通过
File: macro_test1.cpp
		
```cpp
		#include <stdio.h>

		#define B(he...) printf(__VA_ARGS__);
		int main(int argc, const char *argv[])
		{
				B("hello");
		}
		
```

##2015/7/28
Log1:	处理#if宏的时候，如果遇到未定义的宏，则全部解析成0

##2015/7/25
Log1:	总算处理掉#ifdef,#else,#endif三个宏了。。。现在考虑#if,#elif的逻辑怎判断。。。心好塞，这里就要处理逻辑了吗

##2015/7/11
Log1:	项目重开，词法分析器与预编译过程进行合并重写。使用pattern进行重写。方便拓展的那种

##2015/7/7
Log1:	本项目重写

断片：突然在预处理这里卡住了。。。相信很长一段时间不会继续往下写了，因为不知道怎么写啊。。。容我想明白先

##2015/6/30
Log1:	宏定义的连接符号##，对于前后都是参数的情况，会先替换参数。否则会直接连接
Log2:	连接符强制将两个参数黏合在一起。注意是粘合
		File : macro_test.cpp
		
```cpp
		#include <stdio.h>

		#define hello(a,b,c) printf("a %d",b## 3)

		class helcal {
		public:
			helcal ()
			{
			}
			virtual ~helcal (){};
		};

		int main(int argc, const char *argv[])
		{
			int abcdef = 10;
			hello(1,1<<1,2);
			return 0;
		}
```


File end:macro_test.cpp
上述文件将输出
a 8192
(不带回车)为何会出现这个输出，是因为连接符将1<<1跟3连接在了一起，变成了1<<13。所以左移了13位，变成了8192
值得注意的是，这个连接符前后有没有空格并不影响计算结果

Log3:	我觉得我还是重新写一次预编译过程比较好。。。。先不管了，还是按照现在这样写。编译过程对于string的保持不对并不影响编译结果，所以保持不变，不动。
Log4:	总的来说，宏定义的处理不会跨文件
Log5:	注意，除了使用`_VA_ARGS__`以外，还可以自定义变参列表名。。。

##2015/6/29
Log1:	宏函数参数读取结束。现在想想怎么做参数替换。。。直接替换的部分倒是挺简单的，问题是还有#和##。。。

##2015/6/26
Log1:	宏函数的参数说白了也不过是计算表达式。所以我这是要先研究怎么解析计算表达式的节奏吗。。。
		有一个比较折衷的方案就是，直接解析括号，以及尖括号，单引号和双引号对这几对符号进行亚栈配对。其他的符号貌似不会引起逗号的存在。
		只要这两个符号匹配结果正确，就可以压入参数

##2015/6/25
Log1:	wtf函数宏定义展开为何如此烦烦烦烦。。。。。需要进一步探究到底哪些可以做参数哪些不可以

##2015/6/24
Log1:	宏定义对于空格的处理优点特别

```cpp
#define hell (a) //将被认为是一个define，hell的内容将被替换成(a)
#define hell(a) //将被认为是一个define, hell(x)的内容被替换成空白。
```

他们的差异就是中间有没有空格。所以c语言里面的空格还是不能完全忽略的

##2015/6/21
Log1:	重写define mngr，这个东西不能直接返回string。或者说，应该直接返回lex record比较好.

##2015/6/19
Log1:	preprocess过程开始就应该使用语法分析器进行分析了。
		不应该在一个for里面判断一堆if

##2015/6/18
Log1:	真心不知道怎么写了。。。include的头文件扩展之后怎么替换旧的root

##2015/6/17
Log1:	#define允许重定义，但是以最后一个定义为准
Log2:	#define在定义的时候并不关心是否能翻译。只是在展开的时候关心是否已经定义。并且定义里面调用定义也是在展开的时候看的。例如下面的代码是可以编译通过的：
例如下面的代码是可以编译通过的
	File: marco.cpp

```cpp
	#include <stdio.h>

	#define b() a()
	#define a() {printf("hello\n");}
	#define hello a()

	int main(int argc, const char *argv[])
	{
		hello;
		b();
		return 0;
	}
```

File end :marco.cpp

但是另一方面，如果展开的时候出现循环，就会重现无法扩展。原因是宏定义只能展开一次。例如下面的代码无法编译通过:	
	File: marco_error.cpp

```cpp
	#include <stdio.h>
	//间接出现循环
	#define b() a()
	#define a() {printf("hello\n");b();}
	#define hello a()

	int main(int argc, const char *argv[])
	{
		hello;
		b();
		return 0;
	}
```


File: marco_error2.cpp
	
```cpp
	#include <stdio.h>

	//直接循环
	#define a() {printf("hello\n");a();}
	#define hello a()

	int main(int argc, const char *argv[])
	{
		hello;
		return 0;
	}
```
File end :marco_error2.cpp

Log3: 并非所有define都是直接拓展的。下面的代码将会报错：
File marco_error3.cpp
	
```cpp	
	#include <stdio.h>

	#define hello " 123+ 1

	int main(int argc, const char *argv[])
	{
		printf("%s", hello ");
		return 0;
	}
```

File end :marco_error3.cpp

##2015/6/16
Log1:	include并非完全是把头文件拓展开来。\的影响不会从上一个文件继承到下一个文件

##2015/5/25
Log1:	感觉可能不需要独立写注释去处管理。应该在词法分析之后进行注释移除就可以了
Log2:	还是不太明白要怎么处理backSlant


