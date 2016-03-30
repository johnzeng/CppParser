#include <gtest/gtest.h>
#include "Lex.h"
#include "JZFileUtil.h"

TEST(LexUtil, eraseLineSeperator){
  StringList fileList = JZGetAllFiles("./test/Samples/LexUtilEraseLineSeperator", false);
  for(auto i = fileList.begin(); i < fileList.end(); i++)
  {
    uint64 buffSize = 0;
    unsigned char* buff = JZGetFileData(i->c_str(),&buffSize);
    const char* eraseRet = LexUtil::eraseLineSeperator((const char*)buff, &buffSize);
  }
}
