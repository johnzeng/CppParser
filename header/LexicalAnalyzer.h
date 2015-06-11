#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "JZCommonDefine.h"
#include <stack>
using namespace std;

class LexicalAnalyzer;

enum SymbolBlockTypeDef
{
	eSymBlockRoot = 0,			//root node
	eSymBlockGolbal = 1,		//golbal symbol
	eSymBlockNone = 2,			//only surrounded with {}
	eSymBlockFunc = 3,			//func block
	eSymBlockFor = 4,			//for block
	eSymBlockIf	= 5,			//if block
	eSymBlockWhile = 6,			//while block
	eSymBlockDo = 7,			//do block
	eSymBlockClass = 8,			//class block
	eSymBlockNameSpace = 9,		//name space block
	eSymBlockStruct = 10,		//struct block
	eSymBlockEnum = 11,			//enum block
	eSymBlockUnion = 12,		//union block
	eSymBlockTemplate = 13,		//template block
};

struct SymbolNode
{
	SymbolNode* father;
	vector<SymbolNode*> child;
	int blockType;
	string symbolName;
	string symbolType;			//For basic define, symbolType will be the keyword, and no defineNode should be set
	SymbolNode* defineNode;
};

/*
 * Log about LexicalAnalyzer's word
 * 		word will be "\ " if there is a '\' at the end of input(nothing but empty input is followed)
 * */

struct LexicalRecord
{
	LexicalRecord()
	{
		expendAnalyzer = NULL;
		line = 0;
		word = "";
		isConsumed = false;	
	}
	int line;
	string word;
	bool isConsumed;
	LexicalAnalyzer* expendAnalyzer;	//if it is a macro or include mark(anyway, or other expendable keyword) .Otherwise this will be null
} /* optional variable list */;

class LexicalAnalyzer {
public:
	LexicalAnalyzer ();
	virtual ~LexicalAnalyzer ();

	void init();
	void setSourceCodeDir(string path);

	void doAnalyze();

	bool consumeToken(int index);

private:
	void saveAWordAndCleanIt(int line, string& word);
	void saveAWord(int line, const string& word);
	bool isInterpunction(char input);
	bool isEmptyInput(char input);
	bool isEmptyFromIndexTillEnd(const std::string& str, int index);

	// like ++,--,<<,>> 's begin
	bool isDoubleableInterpunction(char input); 

	// like +=, -= 's begin
	bool isSeflEqualInterpunction(char input); 

	// like !=, >= 's begin
	bool isComparorEqualInterpunction(char input); 

	std::vector<LexicalRecord> mRecordList;
	std::string mCodePath;
	StringList mOriginalWords;

	//this node means no symbol,only for root	
	SymbolNode mRootSymbolNode;		
};

class AnalyzerCollector {
public:
	AnalyzerCollector (){};
	virtual ~AnalyzerCollector ();

	static AnalyzerCollector* getInstance();
	
	void addAnalyzer(const std::string& filePath, LexicalAnalyzer* analyzer );
private:
	std::map<std::string, LexicalAnalyzer*> mCollectMap;
};

#endif /* end of include guard: LEXICALANALYZER_H */
