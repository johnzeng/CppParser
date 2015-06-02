#include "DefineManager.h"
#include "ErrorCode.h"

DefineManager::DefineManager()
{
	
}

DefineManager::~DefineManager()
{

}

DefineManager* DefineManager::getInstance()
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
		return JZErrorCode::errDoubleDefineMarco;
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
	return true;
}

const string* DefineManager::findDefineMap(const string& srcDefine)
{
	if (false == isDefined(srcDefine))
	{
		return NULL;
	}
	auto it = mSrcDefineMap.find(srcDefine);
	if (mSrcDefineMap.end() == it)
	{
		return NULL;
	}
	return &(it->second);
}
