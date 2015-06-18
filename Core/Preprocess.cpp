#include "Preprocess.h"
#include "JZLogger.h"
#include "JZMarcoFunc.h"
#include "IncludeHandler.h"
#include "JZFileUtil.h"

Preprocess::Preprocess()
{
	mDefinemanager.init();
}

Preprocess::~Preprocess()
{
}

void Preprocess::init(LexicalAnalyzer* rootLex)
{
	if (rootLex == NULL)
	{
		JZWRITE_DEBUG("no lex is set");
	}
	LexReaderStruct rootReader;
	rootReader.lexPtr = rootLex;
	mLexStack.push(rootReader);
}

void Preprocess::analyze()
{
	while (!mLexStack.empty())
	{
		auto curReader = mLexStack.top();
		mLexStack.pop();
		auto curLex = curReader.lexPtr;
		
		const LexicalRecord* record = NULL;
		while((record = curLex->getLexiRecord(curReader.curIndex))!=NULL)
		{
			//now analyze this record
			if ("#" == record->word)
			{
				//oh! a marco is begining!
				const LexicalRecord* nextRecord = curLex->getLexiRecord(curReader.curIndex);
				curReader.curIndex++;

				if ("define" == nextRecord->word)
				{
					//this is a define
					auto recordList = curLex->getLineRecordTillLineEnd(curReader.curIndex);
					if (recordList.size() == 0)
					{
						JZWRITE_DEBUG("define following nothing!");
						continue;
					}
					std::string keyWord = recordList[0]->word;
					std::string defineWord = "";
					for(int j = 1; j < recordList.size(); j ++)
					{
						defineWord += (recordList[j])->word;
					}
					mDefinemanager.addDefineMap(keyWord, defineWord);	
					JZWRITE_DEBUG("you define word : %s, context is :%s",keyWord.c_str(), defineWord.c_str() );
					curReader.curIndex += recordList.size();
				}
				else if ("include" == nextRecord->word)
				{
					string fileName = "";
					auto suroundMarkA = curLex->getLexiRecord(curReader.curIndex);
					JZIF_NULL_RETURN(suroundMarkA);
					curReader.curIndex++;	
					auto includeRecord = curLex->getLexiRecord(curReader.curIndex);
					JZIF_NULL_RETURN(includeRecord);
					curReader.curIndex++;
					auto suroundMarkB = curLex->getLexiRecord(curReader.curIndex);
					JZIF_NULL_RETURN(suroundMarkB);
					fileName = includeRecord->word;
					string fullPath = IncludeHandler::getInstance()->getFullPathForIncludeFile(fileName);
					if (false == JZFileAccessable(fullPath.c_str()))
					{
						JZWRITE_ERROR("can not access to file : %s",fileName.c_str());
						continue;
					}
					//expand include file
					LexicalAnalyzer* includeLex = AnalyzerCollector::getInstance()->getAnalyzer(fullPath);
					if (NULL == includeLex)
					{
						includeLex = new LexicalAnalyzer();
						includeLex->setSourceCodeDir(fullPath);
						includeLex->doAnalyze();

						AnalyzerCollector::getInstance()->addAnalyzer(fullPath, includeLex);
					}
					mLexStack.push(curReader);
					LexReaderStruct includeReaderStruct;
					includeReaderStruct.lexPtr = includeLex;
					mLexStack.push(includeReaderStruct);
					//break,so you can analyze the new include file
					break;
				}
				else 
				{
					//handle other marco word
				}
			}
			else if(true == mDefinemanager.isDefined(record->word))
			{
				//if this is a defined marco,handle it
				
			}
			else
			{
				//no need to handle
				this->mExpendedList.push_back(*record);	
			}
		
		};
	}
}
