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

}

string IncludeHandler::getFileData(string FullPathName)
{

}

void IncludeHandler::handleEnvIncludePath()
{
	//@todo
}
