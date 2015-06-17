#ifndef _LEXER_H_
#define _LEXER_H_

#include "globalDefs.h"
#include "constantDefs.h"
#include "driver.h"

#include "../tmp/lexerStruct.h"

class Token {
	public:
		// data members
		int tokenType;
		string s;
		unsigned int fileIndex;
		int row;
		int col;
		// allocators/deallocators
		Token(int tokenType = TOKEN_STD);
		Token(int tokenType, const string &s, unsigned int fileIndex, int row, int col);
		Token(const Token &otherToken);
		~Token();
		// operators
		Token &operator=(Token &otherToken);
		operator string() const;
};

vector<Token> *lex(ifstream *in, unsigned int fileIndex);

// post-includes

#include "../tmp/lexerStruct.h"

#endif
