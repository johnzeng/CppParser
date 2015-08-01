#ifndef LEXDATA_H
#define LEXDATA_H

#include <string>
#include "JZCommonDefine.h"
#include <vector>
using namespace std;

typedef vector<string> RealParamList;
typedef stack<int> BracketMarkStack;
typedef map<string,int> ParamSiteMap;
//struct define
struct DefineRec;
struct LexRec
{
	string word;
	uint32 line;
	string file;
	uint32 type;
	uint32 defId;
	uint32 fileType;
	uint32 beginIndex;
	uint32 endIndex;
} ;

enum PSMark
{
	eLexPSIF,
	eLexPSIFDEF,
	eLexPSIFNDEF,
	eLexPSELIF,
	eLexPSELSE,
	eLexPSENDIF,
};

struct PrecompileSelector
{
	uint32 mark;	//like #if,#else,#elif,#endif,and so on
	bool isSuccess;	//if true, the words will be save
	unsigned long tag;
	uint32 beginTag;
};

enum LexRecordType
{
	eLexRecTypeNormal,
	eLexRecTypeConstChar,
	eLexRecTypeString,
	eLexRecTypeFuncLikeMacroParam,
	eLexRecTypeFuncLikeMacroVarParam,
	eLexRecTypeMacro,
};

enum FileReaderRecordType
{
	eFileTypeFile,
	eFileTypeDefine,
	eFileTypeMacro,
	eFileTypeMacroParam,
};

struct FileReaderRecord
{
	const char* buffer;
	uint64 bufferSize;		//should not be change after init
	uint32 curIndex;
	uint32 curLineNum;
	const string fileName;	//if this is a define ,file name will be key
	uint32 recordType;

	//stream tag
	uint32 mStreamOffTag;

	//func like macor analyzing 
	bool mFuncLikeMacroParamAnalyzing;
	BracketMarkStack mBracketMarkStack;
	RealParamList mRealParamList;
	const DefineRec *mDefinePtrStack;
	const ParamSiteMap *mParamSiteMap;
		
};

FileReaderRecord initFileRecord(const char* buff,uint64 size,const string& fileName,uint32 recordType, const RealParamList* paramList = NULL,const DefineRec* defRec = NULL, const ParamSiteMap* paramSiteMap = NULL);

typedef vector<LexRec> LexRecList;
#endif /* end of include guard: LEXDATA_H */
