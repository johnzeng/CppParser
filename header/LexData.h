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
	int line;
	string file;
	uint32 type;
	uint32 defId;
} ;

struct PrecompileSelector
{
	string mark;	//like #if,#else,#elif,#endif,and so on
	bool isSuccess;	//if true, the words will be save
};

enum LexRecordType
{
	eLexRecTypeNormal,
	eLexRecTypeConstChar,
	eLexRecTypeString,
	eLexRecTypeFuncLikeMacroParam,
	eLexRecTypeFuncLikeMacroVarParam,
};

enum FileReaderRecordType
{
	eFileTypeFile,
	eFileTypeDefine,
};

struct FileReaderRecord
{
	const char* buffer;
	uint64 bufferSize;		//should not be change after init
	int curIndex;
	int curLineNum;
	string fileName;	//if this is a define ,file name will be key
	uint32 recordType;

};

typedef vector<LexRec> LexRecList;

#endif /* end of include guard: LEXDATA_H */
