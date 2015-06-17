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
		JZWRITE_ERROR("double define marco, now rewrite it");
	}
	mSrcDefineMap[src] = define;

	return JZErrorCode::errNoError;
}

bool DefineManager::isDefined(const string& srcDefine)
{
	if (mCanceledDefine.end() != mCanceledDefine.find(srcDefine))
	{
		return false;
	}
	if (mSrcDefineMap.end() == mSrcDefineMap.find(srcDefine))
	{
		return false;
	}
	DefineManager* globalInstance = DefineManager::getGlobalInstance();
	if(this != globalInstance)
	{
		//this is not global instance
		bool ret = globalInstance->isDefined(srcDefine);
		if (false == ret)
		{
			return ret;
		}
	}
	return true;
}

const string* DefineManager::findDefineMap(const string& srcDefine)
{
	if (false == isDefined(srcDefine))
	{
		return NULL;
	}

	//for define marco ,use the one that is latest defined
	auto it = mSrcDefineMap.find(srcDefine);
	if (mSrcDefineMap.end() != it)
	{
		return &(it->second);
	}

	DefineManager* globalInstance = DefineManager::getGlobalInstance();
	if (this != globalInstance)
	{
		auto ret = globalInstance->findDefineMap(srcDefine);
		if(NULL != ret)
		{
			return ret;	
		}
	}
	return NULL;
}
