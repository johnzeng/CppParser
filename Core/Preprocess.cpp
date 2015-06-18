#include "Preprocess.h"
#include "JZLogger.h"

Preprocess::Preprocess()
{
	mDefinemanager.init();
}

Preprocess::~Preprocess()
{}

void Preprocess::init(LexicalAnalyzer* rootLex)
{
	if (rootLex == NULL)
	{
		JZWRITE_DEBUG("no lex is set");
	}
	mRootLex = rootLex;
}

void Preprocess::analyze()
{
	const LexicalRecord* record = NULL;
	int i = 0;
	while ((record = mRootLex->getLexiRecord(i)) != NULL)
	{
		i++;

		//now analyze this record
		if ("#" == record->word)
		{
			//oh! a marco is begining!
			const LexicalRecord* nextRecord = mRootLex->getLexiRecord(i);
			i++;

			if ("define" == nextRecord->word)
			{
				//this is a define
				auto recordList = mRootLex->getLineRecordTillLineEnd(i);
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
				i += recordList.size();
			}
			else if ("include" == nextRecord->word)
			{
				string fileName = "";
				auto suroundMarkA = mRootLex->getLexiRecord(i);
				JZIF_NULL_RETURN(suroundMarkA);
			   	i++;	
				auto includeRecord = mRootLex->getLexiRecord(i);
				JZIF_NULL_RETURN(includeRecord);
				i++;
				auto suroundMarkB = mRootLex->getLexiRecord(i);
				JZIF_NULL_RETURN(suroundMarkB);
				fileName = includeRecord->word;
				string fullPath = IncludeHandler->getInstance()->getFullPathForIncludeFile(fileName);
				if (false == JZFileAccessable(fullPath.c_str()))
				{
					JZWRITE_ERROR("can not access to file : %s",fileName.c_str());
					continue;
				}
				//expand include file
				//so here comes the problem...how do I do it.
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
	}
}
