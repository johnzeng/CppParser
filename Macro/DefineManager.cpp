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

DefineManager::DefineManagerReturnCode DefineManager::isDefined(const string& srcDefine)
{
	if (mCanceledDefine.end() != mCanceledDefine.find(srcDefine))
	{
		return eNotDefined;
	}
	if (mSrcLexMap.end() != mSrcLexMap.find(srcDefine))
	{
		return eDefined;
	}
	DefineManager* globalInstance = DefineManager::getGlobalInstance();
	if(this != globalInstance && NULL != globalInstance)
	{
		//this is not global instance
		DefineManagerReturnCode ret = globalInstance->isDefined(srcDefine);
		if (eDefined == ret)
		{
			return ret;
		}
	}
	return eNotDefined ;
}

DefineManager::DefineManagerReturnCode DefineManager::addDefineMap(const string& src, const DefineRec& rec)
{
	if(eDefined == isDefined(src))
	{
		return eDoubleDefineMacro;
	}
	if (rec.key != src)
	{
		return eKeyDiffFormSrc;
	}
	mSrcLexMap[src] = rec;

	return eNoError;
}

const DefineRec* DefineManager::findDefineMap(const string& srcDefine)
{
	if (eNotDefined == isDefined(srcDefine))
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
