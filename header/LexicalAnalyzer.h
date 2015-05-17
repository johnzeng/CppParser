#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "JZCommonDefine.h"
#include <stack>
using namespace std;

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

class LexicalAnalyzer {
public:
	LexicalAnalyzer ();
	virtual ~LexicalAnalyzer ();

	static LexicalAnalyzer* getInstance();

	void init();
	void setSourceCodeDir(string path);

	void doAnalyze(string path);

private:
	StringList mOriginalWords;
	SymbolNode mRootSymbolNode;		//this node means no symbol,only for root	
};

#endif /* end of include guard: LEXICALANALYZER_H */
