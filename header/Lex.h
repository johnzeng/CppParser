#ifndef LEX_H
#define LEX_H

#include <vector>
#include <string>
#include <stack>

using namespace std;

struct LexRec
{
	string word;
	int line;
	string file;
} ;

struct FileReaderRecord
{
	const char* buffer;
	int bufferSize;		//should not be change after init
	int curIndex;
	string fileName;

} ;
typedef vector<LexRec> LexRecList;

class Lex {
public:
	Lex ();
	virtual ~Lex ();
	void analyzeAFile(const string& fileName);

private:
	stack<FileReaderRecord> mReaderStack;
	LexRecList mLexRecList;	
};

#endif /* end of include guard: LEX_H */
