#ifndef CMDINPUTFACTOR_H
#define CMDINPUTFACTOR_H

#include "JZCommonDefine.h"
#include "JZCmdParamHandler.h"

class CmdInputFactor : public JZCmdParamHandler{
public:
	CmdInputFactor ();
	virtual ~CmdInputFactor (){};

	static CmdInputFactor* getInstance();

	virtual void init();
	
	void nonOptHandler(const string& args);

	string getNextFile();
private:
	int mCurIndex;
	StringList mToCompileFile;	
};

#endif /* end of include guard: CMDINPUTFACTOR_H */
