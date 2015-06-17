#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include "JZCommonDefine.h"
#include <stack>
using namespace std;

class LexicalAnalyzer;


/*
 * Log about LexicalAnalyzer's word
 * 		word will be "\ " if there is a '\' at the end of input(nothing but empty input is followed)
 * */
struct LexicalRecord
{
	LexicalRecord()
	{
		expendAnalyzer = NULL;
		line = 0;
		word = "";
		isConsumed = false;	
	}

	int line;
	string word;
	bool isConsumed;
	LexicalAnalyzer* expendAnalyzer;	//if it is a macro or include mark(anyway, or other expendable keyword) .Otherwise this will be null
} /* optional variable list */;

class LexicalAnalyzer {
public:
	LexicalAnalyzer ();
	virtual ~LexicalAnalyzer ();

	void init();
	void setSourceCodeDir(string path);

	void doAnalyze();

	bool consumeToken(int index);

	//if reach end of record,return NULL
	const LexicalRecord* getLexiRecord(int index)const;

	bool setExpandLexiRecord(LexicalAnalyzer* analyzer, int index);

	//this func will handle the annoying \\ symbol
	std::vector<LexicalRecord*>	getLineRecordTillLineEnd(int index);

	void analyzeALine(
			const string& line,int lineNum,
			bool* inStringFlag, bool* inIncludeFlag,
		   	bool* inCommentBlockFlag, bool* inCommentLineFlag,
		   	bool* inCharFlag, bool* backSlantEndFlag);

private:

	void saveAWordAndCleanIt(int line, string& word);
	void saveAWord(int line, const string& word);
	bool isInterpunction(char input);
	bool isEmptyInput(char input);
	bool isEmptyFromIndexTillEnd(const std::string& str, int index);

	// like ++,--,<<,>> 's begin
	bool isDoubleableInterpunction(char input); 

	// like +=, -= 's begin
	bool isSeflEqualInterpunction(char input); 

	// like !=, >= 's begin
	bool isComparorEqualInterpunction(char input); 

	std::vector<LexicalRecord> mRecordList;
	std::string mCodePath;

};

class AnalyzerCollector {
public:
	AnalyzerCollector (){};
	virtual ~AnalyzerCollector ();

	static AnalyzerCollector* getInstance();
	
	void addAnalyzer(const std::string& filePath, LexicalAnalyzer* analyzer );
private:
	std::map<std::string, LexicalAnalyzer*> mCollectMap;
};

#endif /* end of include guard: LEXICALANALYZER_H */
