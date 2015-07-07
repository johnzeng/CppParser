#ifndef DEFINEMANAGER_H
#define DEFINEMANAGER_H

#include "JZCommonDefine.h"
#include "Lex.h"

using namespace std;

/*
 * This class will manage the command line factor, too
 * 
 */

struct DefineRec
{
	string key;
	bool isFuncLikeMacro;
	bool isVarArgs;
	LexRecList defineList;		//if it is an empty macro,this will be empty
	LexRecList formalParam;			//use when this is a funclike macro
} ;

typedef map<string ,DefineRec> DefineRecMap;


class DefineManager {
public:
	enum DefineManagerReturnCode
	{
		eNotDefined = 0,
		eDefined = 1,
		eDoubleDefineMacro = 2,
		eKeyDiffFormSrc = 3,

		//put this at last
		eNoError 
	};

	DefineManager ();
	virtual ~DefineManager ();
	static DefineManager* getGlobalInstance();
	void init();

	//if not defined ,retrun NULL
	const DefineRec* findDefineMap(const string& srcDefine);

	DefineManagerReturnCode isDefined(const string& src);

	//will return error code
	DefineManagerReturnCode addDefineMap(const string& src, const DefineRec& rec);

private:
	DefineRecMap mSrcLexMap;
	StringSet mCanceledDefine;
};

#endif /* end of include guard: DEFINEMANAGER_H */
