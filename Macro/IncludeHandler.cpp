#include "IncludeHandler.h"
#include <stdlib.h>
#include "StringUtil.h"
#include "JZLogger.h"
#include "JZFileUtil.h"

IncludeHandler::IncludeHandler()
{
}

IncludeHandler* IncludeHandler::getInstance()
{
	static IncludeHandler* instance = NULL;
	if (NULL == instance) {
		instance = new IncludeHandler();

	}
	return instance;
}

void IncludeHandler::init()
{
	int headerSearchPathListSize = sizeof(gHeadSearchPath)/sizeof(string);
	JZWRITE_DEBUG("path list size is : %d", headerSearchPathListSize);
	
	for(int i = 0 ; i < headerSearchPathListSize; i ++)
	{
		mSystemIncludePath.push_back(gHeadSearchPath[i]);	
	}
	this->handleEnvIncludePath();
}

//some operator will make some search path unuseable
string IncludeHandler::getFullPathForIncludeFile(string fileName)
{
	//search user path at first
	auto userPathIt = mUserIncludePath.begin();
	for( ; userPathIt != mUserIncludePath.end();  userPathIt++)
	{
		string fullPath = (*userPathIt) + "/" + fileName;
		if (true == JZFileAccessable(fullPath.c_str()))
		{
			return fullPath;
		}
	}

	//search for evn set path
	auto cenvPathIt = mEnvIncludePathForC.begin();
	for( ; cenvPathIt != mEnvIncludePathForC.end() ;cenvPathIt++ )
	{
		string fullPath = (*cenvPathIt) + "/" + fileName;
		if (true == JZFileAccessable(fullPath.c_str()))
		{
			return fullPath;
		}
	}

	auto cppenvPathIt = mEnvIncludePathForCPP.begin();
	for( ; cppenvPathIt != mEnvIncludePathForCPP.end() ;cppenvPathIt++ )
	{
		string fullPath = (*cppenvPathIt) + "/" + fileName;
		if (true == JZFileAccessable(fullPath.c_str()))
		{
			return fullPath;
		}
	}

	auto objcenvPathIt = mEnvIncludePathForOBJC.begin();
	for( ; objcenvPathIt != mEnvIncludePathForOBJC.end() ;objcenvPathIt++ )
	{
		string fullPath = (*objcenvPathIt) + "/" + fileName;
		if (true == JZFileAccessable(fullPath.c_str()))
		{
			return fullPath;
		}
	}

	auto sysPathIt = mSystemIncludePath.begin();
	for(; sysPathIt != mSystemIncludePath.end(); sysPathIt++)
	{
		string fullPath = (*sysPathIt) + "/" + fileName;
		if (true == JZFileAccessable(fullPath.c_str()))
		{
			return fullPath;
		}
	}
	return "";
}

void IncludeHandler::handleEnvIncludePath()
{
	//@todo
	//handle c include path at first
	char *cIncludePath = NULL;
	cIncludePath = getenv(C_INC_ENV_PATH);
	if (NULL != cIncludePath) {

		StringUtil::splitString(cIncludePath, ENV_SEPERATOR, mEnvIncludePathForC);

	}
	char *cppIncludePath = NULL;
	cppIncludePath = getenv(CPP_INC_ENV_PATH);
	if (NULL != cppIncludePath) {

		StringUtil::splitString(cppIncludePath, ENV_SEPERATOR, mEnvIncludePathForCPP);

	}

	char *objCIncludePath = NULL;
	objCIncludePath = getenv(OBJC_INC_ENV_PATH);
	if (NULL != objCIncludePath) {

		StringUtil::splitString(objCIncludePath, ENV_SEPERATOR, mEnvIncludePathForOBJC);

	}
}

void IncludeHandler::addSystemHeaderSearchPath(string path)
{
	if ("" == path)
	{
		return;
	}
	mSystemIncludePath.push_back(path);
}

void IncludeHandler::addUserHeaderSearchPath(string path)
{
	if ("" == path)
	{
		return;
	}
	mUserIncludePath.push_back(path);
}
