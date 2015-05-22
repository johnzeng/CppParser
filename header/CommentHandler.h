#ifndef COMMENTHANDLER_H
#define COMMENTHANDLER_H

/* 
 * This class only handle the comment, should not go into marco define
 */

class CommentHandler {
public:
	CommentHandler ();
	virtual ~CommentHandler ();
	static CommentHandler* getInstance();

	string eraseComment(string src);
};

#endif /* end of include guard: COMMENTHANDLER_H */
