#include "GrammarAnalyzer.h"
#include "LexicalAnalyzer.h"
#include "KeyWordDefine.h"

GrammarAnalyzer::GrammarAnalyzer()
{
	rootLex = NULL;
	isCPPMode = true;

	mRootSymbolNode.blockType = eSymBlockRoot;
}

GrammarAnalyzer::~GrammarAnalyzer()
{
}

void GrammarAnalyzer::init(LexicalAnalyzer* rootLexInput)
{

	this->rootLex = rootLexInput;

/*********************************************************
	init basic symbol key word list 
 ********************************************************/
	int nextIndex = mSymbolList.size();
	SymbolNode basicSymbol;

	basicSymbol.father = &mRootSymbolNode;
	
	basicSymbol.symbolName = C_KEY_WORD_INT;	
	basicSymbol.symbolType = C_KEY_WORD_INT;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

	basicSymbol.symbolName = C_KEY_WORD_CHAR;	
	basicSymbol.symbolType = C_KEY_WORD_CHAR;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

	basicSymbol.symbolName = C_KEY_WORD_LONG;	
	basicSymbol.symbolType = C_KEY_WORD_LONG;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

	basicSymbol.symbolName = C_KEY_WORD_DOUBLE;	
	basicSymbol.symbolType = C_KEY_WORD_DOUBLE;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

	basicSymbol.symbolName = C_KEY_WORD_FLOAT;	
	basicSymbol.symbolType = C_KEY_WORD_FLOAT;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

	basicSymbol.symbolName = C_KEY_WORD_SHORT;	
	basicSymbol.symbolType = C_KEY_WORD_SHORT;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

	basicSymbol.symbolName = CPP_KEY_WORD_BOOL;	
	basicSymbol.symbolType = CPP_KEY_WORD_BOOL;
	basicSymbol.defineNode = NULL;
	mSymbolList.push_back(basicSymbol);
	mRootSymbolNode.child.push_back(&mSymbolList[nextIndex]);	
	nextIndex++;

}

void GrammarAnalyzer::turnOnCppMode(bool turnOn)
{
	isCPPMode = turnOn;
}
