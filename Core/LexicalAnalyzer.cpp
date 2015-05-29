#include "LexicalAnalyzer.h"
#include "JZLogger.h"
#include "JZFileUtil.h"
#include "StringUtil.h"

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
	bool inIncludeFlag = false;
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
						saveAWordAndCleanIt(lineNum, curWord);
						saveAWord(lineNum, "*/");
						i++;
						continue;
					}
				}
				goto LexicalAnalyzer_doAnalyze_addcurWord;
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
			//handle include input
			if (true == inIncludeFlag)
			{
				if ('>' == line[i])
				{
					inIncludeFlag = false;
					saveAWordAndCleanIt(lineNum, curWord);
					saveAWord(lineNum, ">");
					continue;
				}
				goto LexicalAnalyzer_doAnalyze_addcurWord;
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
					saveAWordAndCleanIt(lineNum, curWord);
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
						saveAWordAndCleanIt(lineNum, curWord);
						saveAWord(lineNum, "\\ ");
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
					saveAWordAndCleanIt(lineNum, curWord);
					saveAWord(lineNum, "\"");
					inStringFlag = false;
					continue;
				}
				goto LexicalAnalyzer_doAnalyze_addcurWord;
			}

			//handle inter punction
			if(isInterpunction(line[i]))
			{
				saveAWordAndCleanIt(lineNum, curWord);
				if(i + 1 < line.size())
				{
					JZWRITE_DEBUG("test double length interpunction");
					if (line[i] == '-' && line[i+1] == '>')
					{
						saveAWord(lineNum, "->");
						i++;
						continue;
					}
					else if(line[i] == ':' && line[i+1] == ':')
					{
						saveAWord(lineNum, "::");
						i++;
						continue;
					}
					else if(line[i] == '/' && line[i+1] == '/')
					{
						saveAWord(lineNum, "//");
						i++;
						inCommentLineFlag = true;
						continue;
					}
					else if ('/' == line[i] && '*' == line[i + 1])
					{
						saveAWord(lineNum, "/*");
						i++;
						inCommentBlockFlag = true;
						continue;
					}
					else if ('#' == line[i] && '#' == line[i+1])
					{
						saveAWord(lineNum, "##");
						i++;
						continue;
					}
					//add other doule length inter punction
				}

				//any way check other interpunction
				if (line[i] == '"')
				{
					inStringFlag = true;
					saveAWord(lineNum, "\"");
					continue;
				}
				else if(line[i] == '\'')
				{
					saveAWord(lineNum, "'");	
					inCharFlag = true;
					continue;
				}
				else if (line[i] == '<')
				{
					saveAWord(lineNum, "<");
					inIncludeFlag = true;
					continue;
				}
				else if(!isEmptyInput(line[i]))
				{
					curWord += line[i];
					saveAWordAndCleanIt(lineNum, curWord);
					continue;
				}
				else
				{
					//empty input
					continue;
				}

			}
			if(line[i] == '\\')
			{
				if (isEmptyFromIndexTillEnd(line, i))
				{
					JZWRITE_DEBUG("this is a back slant ,now handle it");
					saveAWordAndCleanIt(lineNum, curWord);
					saveAWord(lineNum, "\\ ");
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
			saveAWordAndCleanIt(lineNum, curWord);
		}
	}

#ifdef DEBUG
	JZWRITE_DEBUG("end of analyze , now print words");
	string outPut = "";
	auto it = mRecordList.begin();	
	for(; it != mRecordList.end() ;it++)
	{
		outPut += "At line : ";
		outPut += StringUtil::tostr(it->line);
		outPut += "\nWord : --" + it->word + "--\n";
	}
	JZWRITE_DEBUG("now write the output : \n%s", outPut.c_str());
#endif
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
	for (int i = index + 1; i < str.size(); i++) {
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
		case '#':
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
void LexicalAnalyzer::saveAWordAndCleanIt(int line, string& word)
{
	saveAWord(line, word);
	word = "";
}
void LexicalAnalyzer::saveAWord(int line, const string& word)
{
	if ("" == word)
	{
		return;
	}
	LexicalRecord record;
	record.line = line;
	record.word = word ;
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
