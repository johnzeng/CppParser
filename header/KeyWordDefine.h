#ifndef __KEY_WORD_DEFINE_H__
#define __KEY_WORD_DEFINE_H__

#include <string>
using namespace std;

//macro 
//for a marco ,it is not necessary to begin wit #keyword,
//it may looks like:
//#   include""
//or
//#   if
const string C_MACRO_WORD_DEFINE = "define";
const string C_MACRO_WORD_IF = "if";
const string C_MACRO_WORD_ELSE = "else";
const string C_MACRO_WORD_ENDIF = "endif";
const string C_MACRO_WORD_IFDEF = "ifdef";
const string C_MACRO_WORD_IFNDEF = "ifndef";
const string C_MACRO_WORD_INCLUDE = "include";

//pragma code
const string COMPILE_CODE_PRAGMA = "#pragma";
const string COMPILE_CODE_WARNING = "warning";
const string COMPILE_CODE_DATA_SEG = "data_seg";
const string COMPILE_CODE_COMMENT = "comment";
const string COMPILE_CODE_HDRSTOP = "hdrstop";
const string COMPILE_CODE_PACKAGE = "package";
const string COMPILE_CODE_RESOURCE = "resource";
const string COMPILE_CODE_MESSAGE = "message";
const string COMPILE_CODE_ONCE = "once";
const string COMPILE_CODE_DISABLE = "disable";

//data type
const string C_KEY_WORD_INT = "int";
const string C_KEY_WORD_CHAR = "char";
const string C_KEY_WORD_UNSIGNED= "unsigned";
const string C_KEY_WORD_LONG= "long";
const string C_KEY_WORD_DOUBLE= "double";
const string C_KEY_WORD_FLOAT= "float";
const string C_KEY_WORD_STRUCT = "struct";
const string C_KEY_WORD_TYPEDEF = "typedef";
const string C_KEY_WORD_ENUM = "enum";
const string C_KEY_WORD_CONST = "const";
const string C_KEY_WORD_AUTO = "auto";
const string C_KEY_WORD_EXTERN = "extern";
const string C_KEY_WORD_REGISTER = "register";
const string C_KEY_WORD_SHORT = "short";
const string C_KEY_WORD_STATIC = "static";
const string C_KEY_WORD_UNION = "union";
const string C_KEY_WORD_SIGNED = "signed";
const string C_KEY_WORD_VOLATILE = "volatile";
const string C_KEY_WORD_SIZEOF = "sizeof";

//routine controll
const string C_KEY_WORD_IF = "if";
const string C_KEY_WORD_FOR = "for";
const string C_KEY_WORD_SWITCH = "switch";
const string C_KEY_WORD_CASE = "case";
const string C_KEY_WORD_WHILE = "while";
const string C_KEY_WORD_DO = "do";
const string C_KEY_WORD_GOTO = "goto";
const string C_KEY_WORD_BREAK = "break";
const string C_KEY_WORD_CONTINUE = "continue";
const string C_KEY_WORD_RETURN = "return";
const string C_KEY_WORD_DEFAULT = "default";

//cpp key word
const string CPP_KEY_WORD_CLASS = "class";
const string CPP_KEY_WORD_NAMESPACE = "namespace";
const string CPP_KEY_WORD_TEMPLATE = "template";
const string CPP_KEY_WORD_PUBLIC = "public";
const string CPP_KEY_WORD_PRIVATE = "private";
const string CPP_KEY_WORD_PROTECTED = "protected";
const string CPP_KEY_WORD_BOOL = "bool";

#endif // __KEY_WORD_DEFINE_H__
