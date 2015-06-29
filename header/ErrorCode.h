#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

namespace JZErrorCode
{
	enum ErrorCode
	{
		errNoError		= 0,
		errUnknow 		= 1,
		errOpenFileFail = 2,
		errNoSuchPath	= 3,
		errDoubleDefineMacro = 4,
		errSharpFollowNothing = 5,
		errSharpFollowWithUnknowKeyWord = 6,
		errSharpDefineFollowWithNothing = 7,
		errSharpIncludeNotSurroundWithRightSeperator = 8,
		errSharpIncludeFollowNoFile = 9,
		errCommentBlockDoEnd = 10,
		errNoDefineIsFind = 11,
		errMissingSeperator = 12,
		errFuncLikeMacroParamError = 13,
		errVarParamAtWrongPose = 14,
		errExpectAnotherSeperator = 15,
		errUnexpectSeperator = 16,
	};
};

#endif //__ERROR_CODE_H__
