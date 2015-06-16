#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

#include <vector>
#include <string>

class LexicalAnalyzer;

enum SymbolBlockTypeDef
{
	eSymBlockUndef = -1,		//undeined,not totally init
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
	SymbolNode()
	{
		father = NULL;
		blockType = eSymBlockUndef;
		defineNode = NULL;		
	}

	SymbolNode* father;
	std::vector<SymbolNode*> child;
	SymbolBlockTypeDef blockType;
	std::string symbolName;
	std::string symbolType;			//For basic define, symbolType will be the keyword, and no defineNode should be set
	SymbolNode* defineNode;
};

class GrammarAnalyzer {
public:
	GrammarAnalyzer ();
	virtual ~GrammarAnalyzer ();
	//don't use singleton.
	
	void init(LexicalAnalyzer* rootLexInput);

	void turnOnCppMode(bool turnOn);

	void doAnalyze();

private:
	
	void expendMarco();

	bool mIsCPPMode;

	LexicalAnalyzer* mRootLex;

	//this node means no symbol,only for root	
	SymbolNode mRootSymbolNode;	
	std::vector<SymbolNode> mSymbolList;
};


#endif /* end of include guard: GRAMMARANALYZER_H */
