#ifndef DEFINEMANAGER_H
#define DEFINEMANAGER_H

#include "JZCommonDefine.h"
#include <map>
#include <string>
#include "LexicalAnalyzer.h"

using namespace std;

/*
 * This class will manage the command line factor, too
 * 
 */
class DefineManager {
public:
	DefineManager ();
	virtual ~DefineManager ();
	static DefineManager* getGlobalInstance();
	void init();

	//if defined ,return defined word, maybe ""
	//if not defined ,retrun NULL
	const vector<LexicalRecord>* findDefineMap(const string& srcDefine);

	bool isDefined(const string& src);

	//will return error code
	int addDefineMap(const string& src, const string& define);
	int addDefineMap(const string& src, const LexRecordList& recList);

private:
//	std::map<string, string> mSrcDefineMap;
	std::map<string, LexRecordList> mSrcLexMap;
	StringSet mCanceledDefine;
};

#endif /* end of include guard: DEFINEMANAGER_H */
