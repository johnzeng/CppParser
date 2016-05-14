#ifndef DEFINEMANAGER_H
#define DEFINEMANAGER_H

#include "JZCommonDefine.h"
#include "LexData.h"

using namespace std;

/*
 * This class will manage the command line factor, too
 * 
 */

class DefineRec
{
public:
	string key;
	bool isFuncLikeMacro;
	bool isVarArgs;
	string defineStr;		//if it is an empty macro,this will be empty
	LexRecList formalParam;		//use when this is a funclike macro
	ParamSiteMap paramMap;

  DefineRec();
} ;

typedef map<string ,DefineRec> DefineRecMap;


class DefineManager {
public:
	enum DefineManagerReturnCode
	{
		eDefMgrNotDefined = 0,
		eDefMgrDefined = 1,
		eDefMgrDoubleDefineMacro = 2,
		eDefMgrKeyDiffFromSrc = 3,

		//put this at last
		eDefMgrNoError 
	};

	DefineManager ();
	virtual ~DefineManager ();

  //I don't think it's a good idea to have global instance for these class now.

	static DefineManager* getGlobalInstance();
	void init();

	//if not defined ,return NULL
	const DefineRec* findDefineMap(const string& srcDefine);

	DefineManagerReturnCode isDefined(const string& src);

	//will return error code
	DefineManagerReturnCode addDefineMap(const string& src, const DefineRec& rec);

  DefineManagerReturnCode cancelDefine(const string& src);

	void printAllDefine();
	uint32 getDefineNum();

private:
	DefineRecMap mSrcLexMap;
	StringSet mCanceledDefine;
};

#endif /* end of include guard: DEFINEMANAGER_H */
