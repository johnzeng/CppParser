#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

class LexicalAnalyzer;

class GrammarAnalyzer {
public:
	GrammarAnalyzer ();
	virtual ~GrammarAnalyzer ();

private:
	LexicalAnalyzer* lex;
};

#endif /* end of include guard: GRAMMARANALYZER_H */
