#ifndef LOOPBREAKER_H
#define LOOPBREAKER_H

#include "JZCommonDefine.h"
using namespace std;

class LoopBreaker {
public:
  LoopBreaker ();
  virtual ~LoopBreaker ();

  bool insert(const string& file, int line, int index);
  bool check(const string& file, int line, int index);
  void remomve(const string& file, int line, int index);

private:
  string getKey(const string& file, int line, int index);
  map<string, int> mLoopMark;
};

#endif /* end of include guard: LOOPBREAKER_H */
