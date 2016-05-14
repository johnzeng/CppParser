#include <gtest/gtest.h>
#include "CommandLineFactorMngr.h"
#include "ErrorCode.h"
#include "JZLogger.h"
#include "JZMacroFunc.h"

using namespace JZErrorCode;

TEST(CommandLineFactorMngr, GetAddonFactor){
  CommandLineFactorMngr* item = new CommandLineFactorMngr();

  item->searchAddonFactorFile();

  EXPECT_EQ(ErrorCode::errNoError, item->addonFactorAnalyser());
  //actually I writed 3 -I in the file ,
  //but we should only get 2 because we -Iheader is same to -I ./header
  EXPECT_LT(2, item->getHeaderFileSearchPath().size());
  EXPECT_LT(2, item->getDefineMap().size());
  
  JZSAFE_DELETE(item);
}

