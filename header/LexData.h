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
	uint32 mark;	          //like #if,#else,#elif,#endif,and so on
	bool isSuccess;	        //if true, the words will be save
	unsigned long tag;	    //mark its index
	uint32 beginTag;	      //mark its beginer index 
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
	vector<PrecompileSelector> mPSStack;
		
};

FileReaderRecord initFileRecord(const char* buff,uint64 size,const string& fileName,uint32 recordType);

typedef vector<LexRec> LexRecList;

#endif /* end of include guard: LEXDATA_H */
