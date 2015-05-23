#ifndef COMMENTHANDLER_H
#define COMMENTHANDLER_H

/* 
 * This class only handle the comment, should not go into marco define
 */
#include "JZCommonDefine.h"

class CommentHandler {
public:
	CommentHandler ();
	virtual ~CommentHandler (){}
	static CommentHandler* getInstance();

	std::string eraseComment(const std::string& src);
};

#endif /* end of include guard: COMMENTHANDLER_H */
