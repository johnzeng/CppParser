#include "IncludeHandler.h"

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
}
