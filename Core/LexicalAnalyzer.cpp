#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
{
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

LexicalAnalyzer* LexicalAnalyzer::getInstance()
{
	static LexicalAnalyzer* instance = NULL;
	if (NULL == instance) {
		instance = new LexicalAnalyzer();
	}
	return instance;
}

void LexicalAnalyzer::init()
{
	mRootSymbolNode.father = NULL;
	mRootSymbolNode.blockType = eSymBlockRoot;
	mRootSymbolNode.symbolName = "";
	mRootSymbolNode.defineNode = NULL;
	mRootSymbolNode.symbolType = "";
}

void LexicalAnalyzer::setSourceCodeDir(string path)
{

}
