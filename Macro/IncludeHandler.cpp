#include "IncludeHandler.h"
#include <stdlib.h>
#include "StringUtil.h"

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
#ifdef _LINUX_

#endif
	this->handleEnvIncludePath();
}

string IncludeHandler::getFullPathForIncludeFile(string fileName)
{
	return "";
}

string IncludeHandler::getFileData(string FullPathName)
{
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
