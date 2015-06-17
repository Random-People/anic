#ifndef _MAIN_DEFS_H_
#define _MAIN_DEFS_H_

// standard includes

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <stack>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::string;
using std::vector;
using std::deque;
using std::map;
using std::stack;
using std::pair;
using std::make_pair;

// class forward declarations

class Tree;
class SymbolTree;
class Type;
class TypeList;
class StdType;
class FilterType;
class StructorListResult;
class StructorList;
class MemberListResult;
class MemberList;
class ObjectType;
class ErrorType;
class TypeStatus;
class IRTree;
	class LabelTree;
	class SeqTree;
	class DataTree;
		class WordTree8;
		class WordTree16;
		class WordTree32;
		class WordTree64;
		class ArrayTree;
		class CompoundTree;
		class TempTree;
		class ReadTree;
	class OpTree;
		class UnOpTree;
		class BinOpTree;
		class ConvTree;
	class CodeTree;
		class LockTree;
		class UnlockTree;
		class CondTree;
		class JumpTree;
		class WriteTree;
		class CopyTree;
		class SchedTree;

// global variable linkage specifiers

extern int optimizationLevel;
extern bool verboseOutput;
extern bool silentMode;
extern bool codedOutput;
extern int tabModulus;
extern bool eventuallyGiveUp;

extern vector<string> inFileNames;

extern int driverErrorCode;
extern int lexerErrorCode;
extern int parserErrorCode;
extern int semmerErrorCode;
extern int gennerErrorCode;

extern Type *nullType;
extern Type *errType;
class StdType;
extern StdType *stdBoolType;
extern StdType *stdIntType;
extern StdType *stdFloatType;
extern StdType *stdCharType;
extern StdType *stdStringType;
extern StdType *stdBoolLitType;

extern StdType *inIntType;
extern StdType *inFloatType;
extern StdType *inCharType;
extern StdType *inStringType;
extern ObjectType *stringerType;
extern ObjectType *boolUnOpType;
extern ObjectType *intUnOpType;
extern ObjectType *boolBinOpType;
extern ObjectType *intBinOpType;
extern ObjectType *floatBinOpType;
extern ObjectType *boolCompOpType;
extern ObjectType *intCompOpType;
extern ObjectType *floatCompOpType;
extern ObjectType *charCompOpType;
extern ObjectType *stringCompOpType;

extern StdType *stdLibType;
extern SymbolTree *stdLib;

extern IRTree *nopCode;

// global function forward declarations

unsigned int getUniqueInt();
string getUniqueId();

#endif
