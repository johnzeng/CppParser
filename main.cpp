#include "JZLogger.h"

int main()
{
	JZLoggerInit();
	JZSetLoggerLevel(JZ_LOG_ALL);
	JZSetLoggerOutPutStatue(1,0);
	JZWRITE_DEBUG("hello");
}
