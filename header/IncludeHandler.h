#ifndef __INCLUDE_HANDLER_H__
#define __INCLUDE_HANDLER_H__

#include <map>
#include <vector>
#include "JZCommonDefine.h"
using namespace std;

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

	StringList SystemIncludePath;
	StringList EnvIncludePathForC;
	StringList EnvIncludePathForCPP;
	StringList EnvIncludePathForOBJC;
};

#endif  // __INCLUDE_HANDLER_H__
