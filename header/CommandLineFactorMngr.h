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
	void setAddonFactorFileDirectory();
	uint32 addonFactorAnalyser();
private:
	string mAddonFactorDefineFile
	FactorList mOriginalFactorList;
}

#endif //__COMMAND_LINE_FACTOR_MNGR_H__
