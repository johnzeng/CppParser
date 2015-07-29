#ifndef LEXDATA_H
#define LEXDATA_H

#include <string>
#include "JZCommonDefine.h"
#include <vector>
using namespace std;

//struct define
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
};

struct FileReaderRecord
{
	const char* buffer;
	uint64 bufferSize;		//should not be change after init
	uint32 curIndex;
	uint32 curLineNum;
	const string fileName;	//if this is a define ,file name will be key
	uint32 recordType;
	uint32 mStreamOffTag;
};

FileReaderRecord initFileRecord(const char* buff,uint64 size,const string& fileName,uint32 recordType);

typedef vector<LexRec> LexRecList;
typedef vector<string> RealParamList;
typedef stack<int> BracketMarkStack;
typedef map<string,int> ParamSiteMap;
#endif /* end of include guard: LEXDATA_H */
