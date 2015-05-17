#ifndef __INCLUDE_HANDLER_H__
#define __INCLUDE_HANDLER_H__

#include <map>
#include <vector>
#include "JZCommonDefine.h"
using namespace std;

#define C_INC_ENV_PATH "C_INCLUDE_PATH"
#define CPP_INC_ENV_PATH "CPLUS_INCLUDE_PATH"
#define OBJC_INC_ENV_PATH "OBJC_INCLUDE_PATH"

class IncludeHandler
{
public:
	IncludeHandler();
	static IncludeHandler* getInstance();

	//For different os,need to write different init func.....I am still confuse about prefix dir.That is a real hard problem.
	void init();

	string getFullPathForIncludeFile(string fileName);
	string getFileData(string FullPathName);

private:
	void handleEnvIncludePath();

	StringList mSystemIncludePath;
	StringList mEnvIncludePathForC;
	StringList mEnvIncludePathForCPP;
	StringList mEnvIncludePathForOBJC;
};

#endif  // __INCLUDE_HANDLER_H__
