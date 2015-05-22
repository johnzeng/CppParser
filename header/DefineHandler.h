#ifndef DEFINEHANDLER_H
#define DEFINEHANDLER_H

#include "JZCommonDefine.h"

#include <map>

//no instance is provide! this class handle one key word a time.
//DefineMngr is needed

class DefineHandler {
public:
	DefineHandler ();
	virtual ~DefineHandler ();

	std::string analyzeDefine(string srcDef);
private:
	StringSet searchedSet;
};

#endif /* end of include guard: DEFINEHANDLER_H */
