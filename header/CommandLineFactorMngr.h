#ifndef __COMMAND_LINE_FACTOR_MNGR_H__
#define __COMMAND_LINE_FACTOR_MNGR_H__

#include <vector>
#include <string>
#include "JZCommonDefine.h"

using namespace std;

typedef vector<string> FactorList;

class CommandLineFactorMngr
{
public:
	//this func may need to rewrite for windows
	void searchAddonFactorFile();
	
	void setAddonFactorFileDirectory(string path);
	uint32 addonFactorAnalyser();
private:
	string mAddonFactorDefineFile
	FactorList mOriginalFactorList;
}

#endif //__COMMAND_LINE_FACTOR_MNGR_H__
