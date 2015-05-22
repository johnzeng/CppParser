#ifndef DEFINEMANAGER_H
#define DEFINEMANAGER_H

#include <map>
#include <string>
using namespace std;

/*
 * This class will manage the command line factor, too
 * 
 */

class DefineManager {
public:
	DefineManager ();
	virtual ~DefineManager ();
	static DefineManager* getInstance();
	void init();

	//if defined ,return defined word, maybe ""
	//if not defined ,retrun NULL
	string* findDefineMap(const string& srcDefine);

	bool isDefined(const string& src);

	void addDefineMap(const string& src, const string& define);

private:
	std::map<string, string> srcDefineMap;		
};

#endif /* end of include guard: DEFINEMANAGER_H */
