#include "DefineManager.h"
#include "ErrorCode.h"
#include "JZLogger.h"
#include "Lex.h"

DefineManager::DefineManager()
{
	
}

DefineManager::~DefineManager()
{

}

DefineManager* DefineManager::getGlobalInstance()
{
	static DefineManager* instance = NULL;
	if (NULL == instance)
	{
		instance = new DefineManager();
	}
	return instance;
}

void DefineManager::init()
{
	//
}

void DefineManager::printAllDefine()
{
	auto it = mSrcLexMap.begin();
	for(; it != mSrcLexMap.end(); it++)
	{
		printf("key is :%s,param num :%d def word is:%s\n", it->first.c_str(),it->second.formalParam.size(),it->second.defineStr.c_str());	
	}
}

DefineManager::DefineManagerReturnCode DefineManager::isDefined(const string& srcDefine)
{
	if (mCanceledDefine.end() != mCanceledDefine.find(srcDefine))
	{
		return eDefMgrNotDefined;
	}
	if (mSrcLexMap.end() != mSrcLexMap.find(srcDefine))
	{
		return eDefMgrDefined;
	}
	DefineManager* globalInstance = DefineManager::getGlobalInstance();
	if(this != globalInstance && NULL != globalInstance)
	{
		//this is not global instance
		DefineManagerReturnCode ret = globalInstance->isDefined(srcDefine);
		if (eDefMgrDefined == ret)
		{
			return ret;
		}
	}
	return eDefMgrNotDefined ;
}

DefineManager::DefineManagerReturnCode DefineManager::addDefineMap(const string& src, const DefineRec& rec)
{
	if(eDefMgrDefined == isDefined(src))
	{
		return eDefMgrDoubleDefineMacro;
	}
	if (rec.key != src)
	{
		return eDefMgrKeyDiffFromSrc;
	}
	mSrcLexMap[src] = rec;
#ifdef DEBUG
	JZWRITE_DEBUG("add define:[key:%s,value:%s]",src.c_str(),rec.defineStr.c_str());
	for(int i = 0 ; i < rec.formalParam.size(); i++)
	{
		JZWRITE_DEBUG("param : %s", rec.formalParam[i].word.c_str());	
	}
#endif
	for(int i = 0; i < rec.formalParam.size() ; i++ )
	{
		string word = rec.formalParam[i].word;	
		mSrcLexMap[src].paramMap[word] = i;
	}
	return eDefMgrNoError;
}

const DefineRec* DefineManager::findDefineMap(const string& srcDefine)
{
	if (eDefMgrNotDefined == isDefined(srcDefine))
	{
		return NULL;
	}

	//for define macro ,use the one that is latest defined
	auto it = mSrcLexMap.find(srcDefine);
	if (mSrcLexMap.end() != it)
	{
		return &(it->second);
	}

	DefineManager* globalInstance = DefineManager::getGlobalInstance();
	if (this != globalInstance && NULL != globalInstance)
	{
		auto ret = globalInstance->findDefineMap(srcDefine);
		if(NULL != ret)
		{
			return ret;	
		}
	}
	return NULL;
}

