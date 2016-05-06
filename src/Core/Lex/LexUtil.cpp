#include "LexUtil.h"
#include "JZLogger.h"
#include <stdlib.h>
#include <string.h>

/*********************************************************
	Lex end here
	now begin the LexUtil   	
 ********************************************************/

bool LexUtil::isInterpunction(const char input)
{
	if (true == isLineEnder(input))
	{
		return true;
	}

	if (true == isEmptyInput(input))
	{
		return true;
	}

	if (true == isBackSlant(input))
	{
		return true;
	}

	switch(input)
	{
		case '|':
		case '%':
		case '^':
		case '=':
		case '+':
		case '/':
		case '*':
		case '-':
		case '~':
		case '\'':
		case '"':
		case ' ':
		case ',':
		case '!':
		case '?':
		case '&':
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case '<':
		case '>':
		case '#':
		case '\t':
		case '\n':
		case ';':
		case ':':
		case '.':
		{
			return true;
		}
		default:
		{
			break;	
		}
	}
	return false;
}

bool LexUtil::isLineEnder(const char input)
{
	if ('\n' == input)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LexUtil::isEmptyInput(const char input)
{
	switch(input)
	{
		case '\t':
		case '\n':
		case ' ':
		case 0:
			return true;
		default:
		{
			return false;
		}
	}
}

bool  LexUtil::isBackSlant(const char input)
{
	if ('\\' == input)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool LexUtil::isEmptyInput(const string& input)
{
	int len = input.size();
	for (int i = 0; i < len; i++) {
		if (false == isEmptyInput(input[i]))
		{
			return false;
		}
	}
	return true;	
}

bool LexUtil::isEndWithBackSlant(const string& input)
{
	bool ret = false;
	for(int32 i = input.size() - 1 ; i >= 0 ; i--)
	{
		if (input[i] == '\\')
		{
			ret = true;
			break;
		}
		else if(false == isEmptyInput(input[i]))
		{
			break;
		}
	}
	return ret;
}

char LexUtil::seperatorMatcher(const char input)
{
	char ret = 0;
	switch(input)
	{
		case '"':
			ret = '"';
			break;
		case '\'':
			ret = '\'';
			break;
		case '[':
			ret = ']';
			break;
		case '<':
			ret = '>';
			break;
		case '(':
			ret = ')';
			break;
		case '{':
			ret = '}';
			break;
		default:
		{
			JZWRITE_DEBUG("not registed input : %c",input);	
			break;
		}
	}
	return ret;
}

char* LexUtil::eraseComment(const char* input,uint64 *bufSize)
{
	JZFUNC_BEGIN_LOG();
	char *ret = (char*)malloc((*bufSize + 1)*sizeof(char));
	uint64 j = 0;
	memset(ret,0,(*bufSize + 1) * sizeof(char));
	bool isString = false;
	bool isChar = false;
	bool lastIsBlant = true;
	bool isCommentLine = false;
	bool isCommentBlock = false;
	for(uint64 i = 0 ; i < (*bufSize); i++)
	{
		if (true == isString)
		{
			ret[j] = input[i];
			if (ret[j] == '"' && false == lastIsBlant)
			{
				isString = false;
			}
			if ('\\' == ret[j])
			{
				lastIsBlant = !lastIsBlant;
			}
			else
			{
				lastIsBlant = false;
			}
      if('\n' == input[i])
      {
        isString = false;
      }
			j++;
		}
		else if(true == isChar)
		{
			ret[j] = input[i];
			if (ret[j] == '\'' && false == lastIsBlant)
			{
				isChar= false;
			}
			if ('\\' == ret[j])
			{
				lastIsBlant = !lastIsBlant;
			}
			else
			{
				lastIsBlant = false;
			}
      if('\n' == input[i])
      {
        isString = false;
      }
			j++;
		
		}
		else if(true == isCommentLine)
		{
			if (input[i] == '\n')
			{
				isCommentLine = false;
				ret[j] = '\n';
				j++;
			}
		}
		else if(true == isCommentBlock)
		{
			if (input[i] == '/' && input[i - 1] == '*')
			{
				isCommentBlock = false;
			}
      else if(input[i] == '\n')
      {
        //don't remove \n inside comment block
        ret[j] = '\n';
        j++;
      }
		}
		else
		{
			if ('"' == input[i])
			{
				isString = true;
			}
			else if ('\'' == input[i])
			{
				isChar = true;
			}
			else if ('/' == input[i])
			{
				if (i+1 < *bufSize)
				{
					if ('/' == input[i+1] )
					{
						isCommentLine = true;
						i++;
						continue;
					}
					else if('*' == input[i+1])
					{
						isCommentBlock = true;
						i++;
            //insert a space into i
            ret[j] = ' ';
            j++;
						continue;
					}
				}
			}
			ret[j] += input[i];
			j++;
		}
	}
	*bufSize = j;
  if (isCommentBlock || isString || isChar)
  {
    JZWRITE_ERROR("not end with plain text");
  }
	JZFUNC_END_LOG();
	return ret;
}

char* LexUtil::eraseLineSeperator(const char* input,uint64 *bufSize, map<long, long> &lineOffsetMap)
{
	JZFUNC_BEGIN_LOG();

  //Bug log: if we init a char point exactly same as the input, it may have some unset memory
  //follows the output, and it make dirty output 
	char *ret = (char*)malloc((*bufSize + 1)*sizeof(char));
	memset(ret,0,(*bufSize + 1) * sizeof(char));

	uint64 j = 0;
	bool isConverBackSlant = false;
  long curLine = 1;

	JZWRITE_DEBUG("buff size is :%lld",*bufSize);
	for(uint64 i = 0 ; i < (*bufSize); i++)
	{
    if('\n' == input[i])
    {
      curLine ++;
    }
		if (false == isBackSlant(input[i]))
		{
			ret[j++] = input[i];
			isConverBackSlant = false;
			continue;
		}
		//check next not empty input
		isConverBackSlant = !isConverBackSlant;
		if (false == isConverBackSlant)
		{
			ret[j++] = '\\';	
			continue;
		}
		int k = i + 1;
		bool endWithBackSlant = true;
		while(k < (*bufSize) && false == isLineEnder(input[k]))
		{
			if (false == isEmptyInput(input[k]))
			{
				endWithBackSlant = false;
				break;
			}
			k++;
		};
		if (true == endWithBackSlant)
		{
      lineOffsetMap[curLine] ++;
      curLine ++;
			i = k;
		}
		else
		{
			ret[j++] = '\\';	
		}
	}
	*bufSize = j;
	JZFUNC_END_LOG();
	return ret;
}

string LexUtil::eatLREmptyInput(const string& toBeEatan)
{
	string ret = "";
	int leftEmptyNum = 0;
	int rightEmptyNum = 0;
	for(int i = 0; i < toBeEatan.size(); i++)
	{
		if (false == isEmptyInput(toBeEatan[i]))
		{
			break;
		}
		leftEmptyNum++;
	}
	for(int i = toBeEatan.size() - 1; i >= 0; i-- )
	{
		if (false == isEmptyInput(toBeEatan[i]))
		{
			break;
		}
		rightEmptyNum++;	
	}
	for(int i  = leftEmptyNum ; i + rightEmptyNum < toBeEatan.size() ; i++)
	{
		ret += toBeEatan[i];	
	}
	return ret;
}

bool LexUtil::canPopCompileStream(uint32 curMark,uint32 toPopMark)
{
	switch(curMark)
	{
		case eLexPSENDIF:
			if (eLexPSENDIF == toPopMark)
			{
				return false;
			}
			return true;
		case eLexPSELSE:
		case eLexPSELIF:
			if (eLexPSENDIF == toPopMark || eLexPSELSE == toPopMark)
			{
				return false;
			}
			return true;
			if (eLexPSENDIF == toPopMark || eLexPSELSE == toPopMark)
			{
				return false;
			}
			return true;
		case eLexPSIF:
		case eLexPSIFDEF:
		case eLexPSIFNDEF:
			return false;
		default:
		{
			break;	
		}
	}
	return false;
}
bool LexUtil::ignoreMacroWhenStreamIsOff(const string& word)
{
	if (
		word != "if" &&
		word != "ifdef" &&
		word != "ifndef" &&
		word != "endif" &&
		word != "elif" &&
		word != "else" 
		)
	{
		return true;	
	}
	return false;
}

//Ah, I think this is stupid... it make this program unable to be multi process, maybe it will be better for each Lex to have a util, otherwise we will have this problem
//void LexUtil::pointIsSeperator(bool state)
//{
//	sPointIsSeperator = state;
//}

bool LexUtil::isConstNumberChar(const char input)
{
	if ('0' <= input && input <= '9')
	{
		return true;
	}
	return false;
}

bool LexUtil::isIdentifyChar(const char input)
{
  if('a' <= input && input <= 'z')
  {
    return true;
  }
  if('A' <= input && input <= 'Z')
  {
    return true;
  }
  if(true == isConstNumberChar(input))
  {
    return true;
  }
  if('_' == input)
  {
    return true;
  }
  return false;
}

/*********************************************************
	LexUtil End here, now begin the lex pattern table 
 ********************************************************/
