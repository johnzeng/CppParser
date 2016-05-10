#ifndef LEX_H
#define LEX_H

#include "MacroLex.h"

using namespace std;

//class define
class Lex:public MacroLex{

public:
  Lex();

public:
	//handler function
  uint32 handleLeftSqualBracket();  // [
  uint32 handleRightSqualBracket(); // ]
	uint32 handleSlant();					// /
	uint32 handlePoint();					//.
	uint32 handleStart();					//*
	uint32 handlePlus();					//+
	uint32 handleMinus();					//-
	uint32 handleUpponSharp();				//^
//	uint32 handleWave();					//~


	uint32 handleMod();						//%
	uint32 handleColon();					//:


private:
};


#endif /* end of include guard: LEX_H */
