#include "LexicalAnalyzer.h"
#include "JZLogger.h"
#include "JZFileUtil.h"
#include "StringUtil.h"

#include <fstream>
using namespace std;

LexicalAnalyzer::LexicalAnalyzer()
{
	mCodePath = "";
}

LexicalAnalyzer::~LexicalAnalyzer()
{
}

void LexicalAnalyzer::init()
{
}

void LexicalAnalyzer::setSourceCodeDir(string path)
{
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
	ifstream iStream;
	iStream.open(mCodePath,ios::in);

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

	while(getline(iStream,line))
	{
		lineNum++;
		JZWRITE_DEBUG("current line is : %s",line.c_str());

		//check error flag statue
		if (false == backSlantEndFlag && true == inStringFlag)
		{
			JZWRITE_ERROR("line %d:didn't end with seperator:\"",lineNum - 1);
		}
		if (false == backSlantEndFlag && true == inCharFlag)
		{
			JZWRITE_ERROR("line %d:didn't end with seperator:'",lineNum - 1);
		}

		//re init flags;
		if(!backSlantEndFlag)
		{
			inCommentLineFlag = false;
			inStringFlag = false;
			inCharFlag = false;
		}
		backSlantEndFlag = false;
		analyzeALine(
				line,lineNum,
				&inStringFlag, &inIncludeFlag,
				&inCommentBlockFlag, &inCommentLineFlag,
				&inCharFlag, &backSlantEndFlag);
	}

#ifdef DEBUG
	JZWRITE_DEBUG("end of analyze , now print words");
	auto it = mRecordList.begin();	
	for(; it != mRecordList.end() ;it++)
	{
		string outPut = "";
		outPut += "At line : ";
		outPut += StringUtil::tostr(it->line);
		outPut += "\nWord : .." + it->word + "..";
		JZWRITE_DEBUG("At line : %d:\nWord:[%s]",it->line, it->word.c_str());
	}
#endif
}

const std::string LexicalAnalyzer::getSrcCodeFileName()
{
	return mCodePath;
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
	if (true == isSeflEqualInterpunction(input))
	{
		return true;
	}
	if (true == isDoubleableInterpunction(input))
	{
		return true;
	}
	if (true == isComparorEqualInterpunction(input))
	{
		return true;
	}
	switch(input)
	{
		case '!':
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
bool LexicalAnalyzer::isComparorEqualInterpunction(char input)
{
	switch(input)
	{
		case '!':
		case '>':
		case '<':
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

bool LexicalAnalyzer::isDoubleableInterpunction(char input)
{
	switch(input)
	{
		case '+':
		case '-':
		case '>':
		case '<':
		case '#':
		case '|':
		case '&':
		case '/':
		case ':':
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

bool LexicalAnalyzer::isSeflEqualInterpunction(char input)
{
	switch(input)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '|':
		case '&':
		case '^':
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
	record.fromAnalyzer = this;
	mRecordList.push_back(record);

}

bool LexicalAnalyzer::consumeToken(int index)
{
	if (index < 0)
	{
		return false;
	}
	if (index >= mRecordList.size())
	{
		return false;
	}
	if (true == mRecordList[index].isConsumed)
	{
		return false;
	}
	mRecordList[index].isConsumed = true;
	return true;
}

const LexicalRecord* LexicalAnalyzer::getLexiRecord(int index)const
{
	if (mRecordList.size() <= index)
	{
		return NULL;
	}
	if (index < 0)
	{
		return NULL;
	}
	return &mRecordList[index];
}

std::vector<LexicalRecord*> LexicalAnalyzer::getLineRecordTillLineEnd(int index)
{
	std::vector<LexicalRecord*> ret;
	int initLine = -1;
	for(int i = index; i < mRecordList.size(); i ++)
	{
		if(initLine == -1)
		{
			initLine = mRecordList[i].line;
		}
		if (mRecordList[i].line != initLine)
		{
			break;
		}
		if(mRecordList[i].word == "\\ ")
		{
			//reset initLine;
			initLine = -1;
			continue;
		}
		ret.push_back(&mRecordList[i]);
	}

	return ret;
}

bool LexicalAnalyzer::setExpandLexiRecord(LexicalAnalyzer* analyzer, int index)
{
	if (index < 0)
	{
		return false;
	}
	if (mRecordList.size() >= index)
	{
		return false;
	}
	if(mRecordList[index].expendAnalyzer != NULL)
	{
		//don't allow double set
		return false;	
	}
	mRecordList[index].expendAnalyzer = analyzer; 
	return true;
}
void LexicalAnalyzer::analyzeALine(
		const string& line,int lineNum,
		bool* inStringFlag, bool* inIncludeFlag,
		bool* inCommentBlockFlag, bool* inCommentLineFlag,
		bool* inCharFlag, bool* backSlantEndFlag)
{
	string curWord = "";
	//now read line
	for(int i = 0 ; i < line.size() ; i ++ )	
	{
		if (i == 0)
		{
			JZWRITE_DEBUG("now begin to analyze this line");
		}
		//handle comment block
		if(*inCommentBlockFlag == true)
		{
			JZWRITE_DEBUG("in comment block");	
			if(i+1 < line.size())
			{
				if(line[i] == '*' && line[i+1] == '/')
				{
					*inCommentBlockFlag = false;	
					saveAWordAndCleanIt(lineNum, curWord);
					saveAWord(lineNum, "*/");
					i++;
					continue;
				}
			}
			goto LexicalAnalyzer_analyzeALine_addCurWord;
		}

		//handle comment line
		if (*inCommentLineFlag == true)
		{
			JZWRITE_DEBUG("in comment line");
			if(line[i] == '\\' && isEmptyFromIndexTillEnd(line, i))
			{
				JZWRITE_DEBUG("end with back slant");
				*backSlantEndFlag = true;
			}
			else
			{
				goto LexicalAnalyzer_analyzeALine_addCurWord;	
			}
		}
		//handle include input
		if (true == *inIncludeFlag)
		{
			JZWRITE_DEBUG("in include flag");
			if ('>' == line[i])
			{
				*inIncludeFlag = false;
				saveAWordAndCleanIt(lineNum, curWord);
				saveAWord(lineNum, ">");
				continue;
			}
			goto LexicalAnalyzer_analyzeALine_addCurWord;
		}
		//handle char input
		if (*inCharFlag == true)
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
				*inCharFlag = false;
				saveAWordAndCleanIt(lineNum, curWord);
				continue;
			}
			goto LexicalAnalyzer_analyzeALine_addCurWord;
		}
		//handle string input
		if(*inStringFlag == true)
		{
			//@todo handle string input	
			if(line[i] == '\\')
			{
				if (isEmptyFromIndexTillEnd(line, i))
				{
					saveAWordAndCleanIt(lineNum, curWord);
					saveAWord(lineNum, "\\ ");
					*backSlantEndFlag = true;
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
				*inStringFlag = false;
				continue;
			}
			goto LexicalAnalyzer_analyzeALine_addCurWord;
		}

		//handle inter punction
		if(isInterpunction(line[i]))
		{
			saveAWordAndCleanIt(lineNum, curWord);
			if(i + 2 < line.size())
			{
				if(line[i] == line[i+1] && line[i+2] == '=')
				{
					if(line[i] == '>' || line[i] == '<')
					{
						JZWRITE_DEBUG("check a triple length word");
						string toSaveWord = "";
						toSaveWord += line[i];
						toSaveWord += line[i+1];
						toSaveWord += line[i+2];
						saveAWord(lineNum, toSaveWord);
						i++;i++;
						continue;	
					}
				}
				else if ('.' == line[i] && line[i] == line[i+1] && line[i] == line[i+2])
				{
					//this is a ver long input
					JZWRITE_DEBUG("check a triple length word");
					string toSaveWord = "";
					toSaveWord += line[i];
					toSaveWord += line[i+1];
					toSaveWord += line[i+2];
					saveAWord(lineNum, toSaveWord);
					i++;i++;
					continue;	
				}
			}
			if(i + 1 < line.size())
			{
				if(line[i] == '/' && line[i+1] == '/')
				{
					//actually ,if you don't handle '//' here,
					//it will jump into isDoubleableInterpunction
					//but that can not make inCommentLineFlag true.
					//so I handle it here
					saveAWord(lineNum, "//");
					i++;
					*inCommentLineFlag = true;
					continue;
				}
				else if ('/' == line[i] && '*' == line[i + 1])
				{
					saveAWord(lineNum, "/*");
					i++;
					*inCommentBlockFlag = true;
					continue;
				}
				else if(
					(true == isDoubleableInterpunction(line[i]) && line[i] == line[i+1])
					||(true == isSeflEqualInterpunction(line[i]) && line[i+1] == '=')
					||(true == isComparorEqualInterpunction(line[i]) && line[i+1] == '=')
					||(line[i] == '-' && line[i+1] == '>')
					)
				{
					string toSaveWord = "";
					toSaveWord += line[i];
					toSaveWord += line[i+1];
					saveAWord(lineNum, toSaveWord);
					i++;
					continue;
				}

			}

			//any way check other interpunction
			if (line[i] == '"')
			{
				*inStringFlag = true;
				saveAWord(lineNum, "\"");
				continue;
			}
			else if(line[i] == '\'')
			{
				saveAWord(lineNum, "'");	
				*inCharFlag = true;
				continue;
			}
			else if (line[i] == '<')
			{
				int index = mRecordList.size() - 1;
				if (index >= 1 
					&& mRecordList[index].word == "include"
					&& mRecordList[index - 1].word == "#")
				{
					//not all < lead to include flag,ok?
					*inIncludeFlag = true;
				}
				saveAWord(lineNum, "<");
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
				while (i+1 < line.size() && true == isEmptyInput(line[i+1]))
				{
					//make sure the next line is empty ,then add 1
					i++;
				}
				saveAWord(lineNum, " ");
				continue;
			}

		}
//		if(line[i] == '\\')
//		{
//			if (isEmptyFromIndexTillEnd(line, i))
//			{
//				JZWRITE_DEBUG("this is a back slant ,now handle it");
//				saveAWordAndCleanIt(lineNum, curWord);
//				saveAWord(lineNum, "\\ ");
//				continue;	
//			}
//			else
//			{
//				JZWRITE_DEBUG("unknow appended after \\");
//			}
//		}
LexicalAnalyzer_analyzeALine_addCurWord:
		//need filter to test if it is all empty
		curWord += line[i];
	}
	if ("" != curWord && " " != curWord)
	{	
		saveAWordAndCleanIt(lineNum, curWord);
	}
}

void LexicalAnalyzer::setRecordList(std::vector<LexicalRecord> list)
{
	JZFUNC_BEGIN_LOG();
	mRecordList = list;
	JZFUNC_END_LOG();
}

const std::vector<LexicalRecord> LexicalAnalyzer::getRecordList()
{
	return mRecordList;
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

LexicalAnalyzer* AnalyzerCollector::getAnalyzer(const std::string& filePath)
{
	auto it = mCollectMap.find(filePath);
	if (mCollectMap.end() == it)
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}

void AnalyzerCollector::addTmpAnalyzer(LexicalAnalyzer* analyzer)
{
	if (NULL == analyzer)
	{
		return;
	}
	mTmpCollectList.push_back(analyzer);
}

void AnalyzerCollector::clearTmpAnalyzer()
{
	for(int i = 0 ;i < mTmpCollectList.size();i++ )
	{
		delete mTmpCollectList[i];	
	}
	mTmpCollectList.clear();
}
//end of collector
