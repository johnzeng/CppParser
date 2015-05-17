#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "JZCommonDefine.h"
#include <stack>
using namespace std;

enum SymbolBlockTypeDef
{
	eSymBlockGolbal = 1,		//golbal symbol
	eSymBlockNone = 2,			//only surrounded with {}
	eSymBlockFunc = 3,			//func block
	eSymBlockFor = 4,			//for block
	eSymBlockIf	= 5,			//if block
	eSymBlockWhile = 6,			//while block
	eSymBlockDo = 7,			//do block
}

struct SymbolNode
{
	SymbolNode* father;
	vector<SymbolNode*> child;
	int blockType;
	string symbolName;
	string symbolType;
	SymbolNode* defineNode;
};

class LexicalAnalyzer {
public:
	LexicalAnalyzer ();
	virtual ~LexicalAnalyzer ();

	static LexicalAnalyzer* getInstance();

	void init();
	void setSourceCodeDir(string path);
	
private:
	StringList mOriginalWords;
	
};

#endif /* end of include guard: LEXICALANALYZER_H */
