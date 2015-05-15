#include "CommandLineFactorMngr.h"
#include <string>
#include "JZLogger.h"
#include "ErrorCode.h"
using namespace std;
using namespace JZErrorCode;

uint32 CommandLineFactorMngr::handleBarD(string param)
{
	string wholeParam = param.substr(2);
	int firstEqual = wholeParam.find_first_of("=");
	JZWRITE_DEBUG("firequal is : %d", firstEqual);

	if (string::npos == firstEqual) {
		//no equal is set,only define
		mDefMap[wholeParam] = "";
		JZWRITE_DEBUG("add define : %s", wholeParam.c_str() );
	}else{
		string key = wholeParam.substr(0, firstEqual);
		string value = wholeParam.substr(firstEqual + 1, wholeParam.size() - firstEqual);
		mDefMap[key] = value;
		JZWRITE_DEBUG("add define=value : %s = %s", key.c_str(), value.c_str());
	}

	return errNoError;
}
