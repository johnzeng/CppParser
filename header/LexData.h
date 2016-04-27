#ifndef LEXDATA_H
#define LEXDATA_H

#include <string>
#include "JZCommonDefine.h"
#include <vector>
using namespace std;

typedef vector<string> RealParamList;
typedef stack<int> BracketMarkStack;
typedef stack<int> ParamSeperaterMarkStack;
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
  //it's a little mess up here
  //need to define different file type for:
  //1, normal file
  //2, macro param type
  //3, macro if type
  //4, expending macro
	eFileTypeFile,
	eFileTypeMacroDefine,
  eFileTypeMacroIf,
	eFileTypeMacroExpand,
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

  //ok,this is a little stupid now, we are going to do another lex
	vector<PrecompileSelector> mPSStack;
		
};

//FileReaderRecord initFileRecord(const char* buff,uint64 size,const string& fileName,uint32 recordType);

typedef vector<LexRec> LexRecList;

#endif /* end of include guard: LEXDATA_H */
