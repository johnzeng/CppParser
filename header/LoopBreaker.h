#ifndef LOOPBREAKER_H
#define LOOPBREAKER_H

class LoopBreaker {
public:
  LoopBreaker ();
  virtual ~LoopBreaker ();

  void insert(const string& file, int line, int index);
  bool check(const string& file, int line, int index);
  void remomve(const string& file, int line, int index);

private:
  map<string, int> mLoopMark;
};

#endif /* end of include guard: LOOPBREAKER_H */
