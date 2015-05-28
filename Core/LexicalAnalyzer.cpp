#include "LexicalAnalyzer.h"
#include "JZLogger.h"
#include "JZFileUtil.h"

#include <sstream>
using namespace std;

LexicalAnalyzer::LexicalAnalyzer()
{
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

void LexicalAnalyzer::init()
{
	mRootSymbolNode.father = NULL;
	mRootSymbolNode.blockType = eSymBlockRoot;
	mRootSymbolNode.symbolName = "";
	mRootSymbolNode.defineNode = NULL;
	mRootSymbolNode.symbolType = "";

	mCodePath = "";
}

void LexicalAnalyzer::setSourceCodeDir(string path)
{
	//@todo
	if(false == JZFileAccessable(path.c_str()))
	{
		JZWRITE_DEBUG("can not access file path");
		return;
	}
	mCodePath = path;
}

void LexicalAnalyzer::doAnalyze()
{
	//@todo
	if ("" == mCodePath)
	{
		JZWRITE_DEBUG("code path is not set ,now return");
		return;
	}
	char* inputChar = (char*)JZGetFileData(mCodePath.c_str());
	stringstream ioStream;
	ioStream << inputChar;

	//ioStream init finished.Now read line
	string line;
	string curWord;
	
	bool inStringFlag = false;
	bool inCommentBlockFlag = false;
	bool inCommentLineFlag = false;
	bool inCharFlag = false;
	bool backSlantEndFlag = false;
	int lineNum = 0;

	while(false != getline(ioStream,line))
	{
		lineNum ++;
		JZWRITE_DEBUG(line.c_str());
		curWord = "";

		//re init flags;
		if(!backSlantEndFlag)
		{
			inCommentLineFlag = false;
			inStringFlag = false;
			inCharFlag = false;
		}
		backSlantEndFlag = false;

		//now read line
		for(int i = 0 ; i < line.size() ; i ++ )	
		{

			//handle comment block
			if(inCommentBlockFlag == true)
			{
				JZWRITE_DEBUG("in comment block");	
				if(i+1 < line.size())
				{
					if(line[i] == '*' && line[i+1] == '/')
					{
						inCommentBlockFlag = false;	
						saveAWrodAndCleanIt(lineNum, curWord);
						saveAWrod(lineNum, "*/");
						i++;
						continue;
					}
				}
			}

			//handle comment line
			if (inCommentLineFlag == true)
			{
				JZWRITE_DEBUG("in comment line");
				if(line[i] == '\\' && isEmptyFromIndexTillEnd(line, i))
				{
					JZWRITE_DEBUG("end with back slant");
					backSlantEndFlag = true;
				}
				else
				{
					goto LexicalAnalyzer_doAnalyze_addcurWord;	
				}
			}
			//handle char input
			if (inCharFlag == true)
			{
				if(line[i] == '\\' && line.size() > i+1)	
				{
					//eat next input
					curWord += line[i];
					curWord += line[i+1];
					i++;
					continue;	
				}
				if(line[i] == '\'')
				{
					inCharFlag = false;
					saveAWrodAndCleanIt(lineNum, curWord);
					continue;
				}
				goto LexicalAnalyzer_doAnalyze_addcurWord;
			}
			//handle string input
			if(inStringFlag == true)
			{
				//@todo handle string input	
				if(line[i] == '\\')
				{
					if (isEmptyFromIndexTillEnd(line, i))
					{
						saveAWrodAndCleanIt(lineNum, curWord);
						saveAWrod(lineNum, "\ ");
						backSlantEndFlag = true;
						break;
					}
					else
					{
						//eat the next char
						if(i+1 < line.size())
						{
							curWord += line[i];
							curWord += line[i+1];	
							i++;
							continue;
						}
						else
						{
							JZWRITE_DEBUG("unlikely to reach here");	
						}
					}
					JZWRITE_DEBUG("not in the end of input");
				}
				else if(line[i] == '\"')
				{
					saveAWrod(lineNum, "\"");		
					inStringFlag = false;
				}
				goto LexicalAnalyzer_doAnalyze_addcurWord;
			}

			//handle inter punction
			if(isInterpunction(line[i]))
			{
				saveAWrodAndCleanIt(lineNum, curWord);
				if(i + 1 < line.size())
				{
					JZWRITE_DEBUG("test double length interpunction");
					if (line[i] == '-' && line[i+1] == '>')
					{
						saveAWrod(lineNum, "->");
						i++;
						continue;
					}
					else if(line[i] == ':' && line[i+1] == ':')
					{
						saveAWrod(lineNum, "::");
						i++;
						continue;
					}
					else if(line[i] == '/' && line[i+1] == '/')
					{
						saveAWrod(lineNum, "//");
						i++;
						inCommentLineFlag = true;
						continue;
					}
					//add other doule length inter punction
				}

				//any way check other interpunction
				if (line[i] == '"')
				{
					inStringFlag = true;
					saveAWrod(lineNum, "\"");
					continue;
				}
				else if(line[i] == '\'')
				{
					saveAWrod(lineNum, "'");	
					inCharFlag = true;
					continue;
				}

			}
			if(line[i] == '\\')
			{
				if (isEmptyFromIndexTillEnd(line, i))
				{
					JZWRITE_DEBUG("this is a back slant ,now handle it");
					saveAWrodAndCleanIt(lineNum, curWord);
					saveAWrod(lineNum, "\ ");
					continue;	
				}
				else
				{
					JZWRITE_DEBUG("unknow appended after \\");
				}
			}
LexicalAnalyzer_doAnalyze_addcurWord:
			//need filter to test if it is all empty
			curWord+=line[i];
		}
		if ("" != curWord)
		{	
			JZWRITE_DEBUG("save a word at the end of line");
			saveAWrodAndCleanIt(lineNum, curWord);
		}
	}
}

bool LexicalAnalyzer::isEmptyInput(char input)
{
	switch(input)
	{
		case '\t':
		case ' ':
		{
			return true;
			break;
		}
		default:
		{
			return false;
			break;	
		}
	}
}

bool LexicalAnalyzer::isEmptyFromIndexTillEnd(const std::string& str, int index)
{
	//@todo
	for (int i = 0; i < str.size(); i++) {
		if(false == isEmptyInput(str[i]))
		{
			return false;	
		}
	}
	
	return true;
}

bool LexicalAnalyzer::isInterpunction(char input)
{
	//@todo
	if (true == isEmptyInput(input))
	{
		return true;
	}
	switch(input)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '{':
		case '}':
		case '[':
		case ']':
		case '\'':
		case '\"':
		case ';':
		case ':':
		case '~':
		case '^':
		case '|':
		case '&':
		case '<':
		case '>':
		case ',':
		case '.':
		case '?':
		case '%':
		case '=':
		{
			return true;
			break;	
		}	
		default:
		{
			return false;
			break;	
		}
	}
	return false;
}
void LexicalAnalyzer::saveAWrodAndCleanIt(int line, string& word)
{
	saveAWord(line, word);
	word = "";
}
void LexicalRecord::saveAWord(line, const string word)
{
	LexicalRecord record;
	record.line = 0;
	record.word =word 
	expendList = NULL;
	mRecordList.push_back(record);

}
//end of Lexical analyzer

AnalyzerCollector::~AnalyzerCollector ()
{
}

AnalyzerCollector* AnalyzerCollector::getInstance()
{
	static AnalyzerCollector* instance = NULL;
	if (NULL == instance)
	{
		instance = new AnalyzerCollector();
	}
	return instance;
}

void AnalyzerCollector::addAnalyzer(const std::string& filePath, LexicalAnalyzer* analyzer)
{
	auto it = mCollectMap.find(filePath);
	if (mCollectMap.end() == it)
	{
		mCollectMap[filePath] = analyzer;
	}
	else
	{
		JZWRITE_DEBUG("double use of analyzer");
	}
}
//end of collector
