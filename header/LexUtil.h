#ifndef LEXUTIL_H
#define LEXUTIL_H

#include "DefineManager.h"
namespace LexUtil {
	char* eraseComment(const char* input, uint64 *bufSize);
	char* eraseLineSeperator(const char* input,uint64 *bufSize, map<long,long>& lineOffsetMap);
	bool isInterpunction(const char input);
	bool isLineEnder(const char input);
	bool isEmptyInput(const char input);
	bool isBackSlant(const char input);
	bool isEmptyInput(const string& input);
	bool isEndWithBackSlant(const string& input);
	char seperatorMatcher(const char input);
	bool canPopCompileStream(uint32 curMark,uint32 toPopMark);
	bool ignoreMacroWhenStreamIsOff(const string& word);
	bool isConstNumberChar(const char input);
	string eatLREmptyInput(const string &toBeEatan);
  bool isIdentifyChar(const char input);

} /* LexUtil */


#endif /* end of include guard: LEXUTIL_H */
