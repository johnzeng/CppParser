#include "Lex.h"
#include "JZLogger.h"
#include "JZFileUtil.h"
#include "JZMacroFunc.h"
#include "IncludeHandler.h"
#include "KeyWordDefine.h"
#include <string.h>
#include <stdlib.h>
#include "LexPatternTable.h"

Lex::Lex(){
  MacroLex();
	/*********************************************************
		init pattern map here 
	 ********************************************************/
	mPatternTable->insertPattern('/',  (LexPatternHandler)(&Lex::handleSlant));
	mPatternTable->insertPattern('.',  (LexPatternHandler)(&Lex::handlePoint));
	mPatternTable->insertPattern('*',  (LexPatternHandler)(&Lex::handleStart));
	mPatternTable->insertPattern('+',  (LexPatternHandler)(&Lex::handlePlus));
	mPatternTable->insertPattern('-',  (LexPatternHandler)(&Lex::handleMinus));
	mPatternTable->insertPattern('~',  (LexPatternHandler)(&Lex::handleWave));
	mPatternTable->insertPattern('%',  (LexPatternHandler)(&Lex::handleMod));
	mPatternTable->insertPattern('^',  (LexPatternHandler)(&Lex::handleUpponSharp));
	mPatternTable->insertPattern(':',  (LexPatternHandler)(&Lex::handleColon));

}

uint32 Lex::handlePoint()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = ".";
	if ('.' == nextChar)
	{
		nextChar = 0;
		ret = readChar(&nextChar);
		if ('.' == nextChar)
		{
			//consume two char:".."
			consumeChar(&nextChar);
			consumeChar(&nextChar);
			toSave = "...";
		}
	}else if ('*' == nextChar)
	{
		//.*
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleMinus()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "-";
	if (
		'-' == nextChar||	//--
		'>' == nextChar||	//->
		'=' == nextChar)	//-=
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	if ("->" == toSave)
	{
		ret = readChar(&nextChar);
		if ('*' == nextChar) //->*
		{
			toSave += nextChar;
		}
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleWave()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "~";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
//	saveWord(toSave);
	return ret;
}

uint32 Lex::handleUpponSharp()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "^";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleColon()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = ":";
	if (':' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;

}

uint32 Lex::handleMod()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "%";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;

}


uint32 Lex::handlePlus()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "+";
	if (
		'+' == nextChar||
		'=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleStart()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "*";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
//	saveWord(toSave);
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

uint32 Lex::handleSlant()
{
	uint32 beginIndex = getLastIndex();
	uint32 ret = eLexNoError;
	char nextChar = 0;
	ret = readChar(&nextChar);
	string toSave = "/";
	if ('=' == nextChar)
	{
		consumeChar(&nextChar);
		toSave += nextChar;
	}
	uint32 endIndex = getLastIndex();
	saveWord(toSave,beginIndex,endIndex);
	return ret;
}

