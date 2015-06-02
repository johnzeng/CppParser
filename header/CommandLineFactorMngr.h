#ifndef __COMMAND_LINE_FACTOR_MNGR_H__
#define __COMMAND_LINE_FACTOR_MNGR_H__

#include <vector>
#include <string>
#include <map>
#include "JZCommonDefine.h"
using namespace std;

#define DEFAULT_COMMON_LINE_FACTOR_FILE_NAME ".factor"


//Currently, I think that -D param can only define a word,no marco function is concerned.
//BTW, value can be empty, it turns out to be ""
typedef map<string ,string> DefineMarcoMap;

class CommandLineFactorMngr
{
public:
	CommandLineFactorMngr();

	static CommandLineFactorMngr* getInstance();

	//this func may need to rewrite for windows
	void searchAddonFactorFile();
	
	void setAddonFactorFileDirectory(string path);

	uint32 addonFactorAnalyser();

	uint32 handleBarD(string param);
	uint32 handleBarI(string param);

	const StringSet& getHeaderFileSearchPath();
	const DefineMarcoMap& getDefineMap();

private:
	string 			mAddonFactorDefineFile;
	string			mAddonFactorFileDirectory;
	StringList		mOriginalFactorList;
	DefineMarcoMap 	mDefMap;
	StringSet		mHeaderFileSearchPath;
};

#endif //__COMMAND_LINE_FACTOR_MNGR_H__
