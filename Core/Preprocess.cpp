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
	LexicalRecord* record = NULL;
	int i = 0;
	while ((record = mRootLex->getLexiRecord(i)) != NULL)
	{
		i++;

		//now analyze this record
		if ("#" == record->word)
		{
			//oh! a marco is begining!
			LexicalRecord* nextRecord = mRootLex->getLexiRecord(i);
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
					defineword += (recordptrit[j])->word;
					defineword += " ";
				}
				JZWRITE_DEBUG("you define word : %s, context is :%s", );
			}
		}
	}
}
