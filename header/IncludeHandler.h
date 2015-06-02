#ifndef __INCLUDE_HANDLER_H__
#define __INCLUDE_HANDLER_H__

#include <map>
#include <vector>
#include "JZCommonDefine.h"
using namespace std;

#define C_INC_ENV_PATH "C_INCLUDE_PATH"
#define CPP_INC_ENV_PATH "CPLUS_INCLUDE_PATH"
#define OBJC_INC_ENV_PATH "OBJC_INCLUDE_PATH"

//there is a very large problem that
//I can not get the include search path in my engine...
//I can just try now...
const string gHeadSearchPath[] = {
	"/usr/include/",
	"/usr/local/include/"
};

class IncludeHandler
{
public:
	IncludeHandler();
	static IncludeHandler* getInstance();

	//For different os,need to write different init func.....I am still confuse about prefix dir.That is a real hard problem.
	void init();

	string getFullPathForIncludeFile(string fileName);

	void addSystemHeaderSearchPath(string path);
	void addUserHeaderSearchPath(string path);

private:
	void handleEnvIncludePath();

	StringList mUserIncludePath;
	StringList mSystemIncludePath;
	StringList mEnvIncludePathForC;
	StringList mEnvIncludePathForCPP;
	StringList mEnvIncludePathForOBJC;
};

#endif  // __INCLUDE_HANDLER_H__
