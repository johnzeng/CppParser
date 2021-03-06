#include <gtest/gtest.h>
#include "JZLogger.h"

void globalInitAtBegin();

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
	//init
	globalInitAtBegin();

  JZSetLoggerLevel(JZ_LOG_TEST);
  JZSetLoggerOutPutStatue(1,1);
  JZSetLogFileName("logs/test.log");
  return RUN_ALL_TESTS();
}
