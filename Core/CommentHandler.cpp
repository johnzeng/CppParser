#include "CommentHandler.h"
#include <sstream>
#include "JZLogger.h"

using namespace std;

#define COMMENT_FLAG_NON 0
#define COMMENT_FLAG_SRC 1
#define COMMENT_FLAG_LINE_COMMENT 2
#define COMMENT_FLAG_BLOCK_COMMENT 3
#define COMMENT_FLAG_END 4
#define COMMENT_FLAG_STR 5

CommentHandler::CommentHandler()
{
}

CommentHandler* CommentHandler::getInstance()
{
	static CommentHandler* instance = NULL;
	if (NULL == instance)
	{
		instance = new CommentHandler();
	}
	return instance;
}

std::string CommentHandler::eraseComment(const std::string& src)
{
	int flag = COMMENT_FLAG_NON;
	stringstream ioStream;
	ioStream << src;
	string line;
	while(false != getline(ioStream, line))
	{
		JZWRITE_DEBUG("get line : %s", line.c_str());
		for (int i = 0; i < line.size; i++) {
			if ('\\' == line[i])
			{
				//handle "\" here

			}
			if('"' == line[i] && (flag == COMMENT_FLAG_NON || flag == COMMENT_FLAG_SRC))
			{
				//in src code ,now enter the str mode
				flag = COMMENT_FLAG_STR;
				continue;	
			}

			switch(flag)
			{
				case COMMENT_FLAG_NON:
				{
					break;	
				}
				default:
				{
					break;	
				}
			}			
		}
		
	}
	return "";
}
