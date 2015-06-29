#include "DefineManager.h"
#include "ErrorCode.h"
#include "JZLogger.h"

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

int DefineManager::addDefineMap(const string& src, const string& define)
{
	if (true == isDefined(src))
	{
		JZWRITE_ERROR("double define macro, now rewrite it");
	}

//	mSrcLexMap[src] = define;
	LexRecordList list;
	if ("" != define)
	{
		LexicalRecord record;
		record.word = define;
		list.push_back(record);
	}

	this->addDefineMap(src, list);

	return JZErrorCode::errNoError;
}

bool DefineManager::isDefined(const string& srcDefine)
{
	if (mCanceledDefine.end() != mCanceledDefine.find(srcDefine))
	{
		return false;
	}
	if (mSrcLexMap.end() != mSrcLexMap.find(srcDefine))
	{
		return true;
	}
	DefineManager* globalInstance = DefineManager::getGlobalInstance();
	if(this != globalInstance && NULL != globalInstance)
	{
		//this is not global instance
		bool ret = globalInstance->isDefined(srcDefine);
		if (true == ret)
		{
			return ret;
		}
	}
	return false;
}

int DefineManager::addDefineMap(const string& src, const LexRecordList& recList)
{
	if(true == isDefined(src))
	{
		return JZErrorCode::errDoubleDefineMacro;
	}
	mSrcLexMap[src] = recList;

	return JZErrorCode::errNoError;
}

const vector<LexicalRecord>* DefineManager::findDefineMap(const string& srcDefine)
{
	if (false == isDefined(srcDefine))
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
