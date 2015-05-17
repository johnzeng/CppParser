#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
{
}

LexicalAnalyzer* LexicalAnalyzer::getInstance()
{
	static LexicalAnalyzer* instance = NULL;
	if (NULL == instance) {
		return instance;
	}
}

void LexicalAnalyzer::init()
{

}

