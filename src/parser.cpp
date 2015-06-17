#include "parser.h"

#include "outputOperators.h"

// parser-global variables

int parserErrorCode;

// Tree functions

// constructors
Tree::Tree(const Token &t) : t(t), next(NULL), back(NULL), child(NULL), parent(NULL), status(TypeStatus()) {}
Tree::Tree(const Token &t, Tree *next, Tree *back, Tree *child, Tree *parent) : t(t), next(next), back(back), child(child), parent(parent), status(TypeStatus()) {}
Tree::Tree(const TypeStatus &status) : next(NULL), back(NULL), child(NULL), parent(NULL), status(status) {}
Tree::Tree(deque<unsigned int> *depthList) : t(TOKEN_RAW_VECTOR), next((Tree *)depthList) {}

// destructor
Tree::~Tree() {
	delete next;
	delete child;
}

// core methods
IRTree *Tree::code() const {
	return (status.code);
}
Type &Tree::typeRef() const {
	return (*(status.type));
}
DataTree *Tree::castCode(const Type &destType) const {
	return (status.castCode(destType));
}
DataTree *Tree::castCommonCode(const Type &otherType) const {
	return (status.castCommonCode(otherType));
}

// comparison oparators
bool Tree::operator==(int tokenType) const {
	return (t.tokenType == tokenType);
}
bool Tree::operator!=(int tokenType) const {
	return (t.tokenType != tokenType);
}

// traversal operators
// binary traversers
Tree *Tree::goNext(unsigned int n) const {
	const Tree *cur = this;
	for (unsigned int i=0; i<n; i++) {
		if (cur != NULL) {
			cur = cur->next;
		} else {
			return NULL;
		}
	}
	return (Tree *)cur;
}
Tree *Tree::goBack(unsigned int n) const {
	const Tree *cur = this;
	for (unsigned int i=0; i<n; i++) {
		if (cur != NULL) {
			cur = cur->back;
		} else {
			return NULL;
		}
	}
	return (Tree *)cur;
}
Tree *Tree::goChild(unsigned int n) const {
	const Tree *cur = this;
	for (unsigned int i=0; i<n; i++) {
		if (cur != NULL) {
			cur = cur->child;
		} else {
			return NULL;
		}
	}
	return (Tree *)cur;
}
Tree *Tree::goParent(unsigned int n) const {
	const Tree *cur = this;
	for (unsigned int i=0; i<n; i++) {
		if (cur != NULL) {
			cur = cur->parent;
		} else {
			return NULL;
		}
	}
	return (Tree *)cur;
}
Tree *Tree::bottom() const {
	const Tree *cur = this;
	while (cur->child != NULL) {
		cur = cur->child;
	}
	return (Tree *)cur;
}

// binary attatchers
void Tree::operator+=(Tree *next) {
	this->next = next;
}
void Tree::operator-=(Tree *back) {
	this->back = back;
}
void Tree::operator*=(Tree *child) {
	this->child = child;
}
void Tree::operator&=(Tree *parent) {
	this->parent = parent;
}

// converters
Tree::operator string() const {
	if (*this == TOKEN_NonArrayedIdentifier || *this == TOKEN_ArrayedIdentifier || *this == TOKEN_OpenIdentifier) { // if this is an identifier-style Tree node, decode it
		string retVal(child->t.s); // ID or DPERIOD
		// invariant: cur is a non-NULL child of NonArrayedIdentifierSuffix, ArrayedIdentifierSuffix, or IdentifierSuffix
		for(const Tree *cur = child->next->child; cur != NULL; cur = (cur->next->next != NULL) ? cur->next->next->child : NULL) {
			// log the extension
			const Tree *curn = cur->next; // ID, ArrayAccess, or TIMES
			if (*curn == TOKEN_ID) {
				retVal += '.';
				retVal += curn->t.s;
			} else if (*curn == TOKEN_ArrayAccess) {
				// check to make sure that the expressions are compatible with STD_INT
				if (curn->child->next->next->next == NULL) { // if there's only one subscript
					TypeStatus expStatus = getStatusExp(curn->child->next);
					if (!(*expStatus >> *stdIntType)) { // if the types are incompatible, flag an error
						Token curToken = curn->child->next->t; // Exp
						semmerError(curToken.fileIndex,curToken.row,curToken.col,"array subscript is invalid");
						semmerError(curToken.fileIndex,curToken.row,curToken.col,"-- (subscript type is "<<expStatus<<")");
					}
					retVal += ".[]";
				} else { // else if this is an extent subscript
					TypeStatus leftExpStatus = getStatusExp(curn->child->next);
					if (!(*leftExpStatus >> *stdIntType)) { // if the types are incompatible, flag an error
						Token curToken = curn->child->next->t; // Exp
						semmerError(curToken.fileIndex,curToken.row,curToken.col,"left extent subscript is invalid");
						semmerError(curToken.fileIndex,curToken.row,curToken.col,"-- (subscript type is "<<leftExpStatus<<")");
					}
					TypeStatus rightExpStatus = getStatusExp(curn->child->next);
					if (!(*rightExpStatus >> *stdIntType)) { // if the types are incompatible, flag an error
						Token curToken = curn->child->next->next->next->t; // Exp
						semmerError(curToken.fileIndex,curToken.row,curToken.col,"right extent subscript is invalid");
						semmerError(curToken.fileIndex,curToken.row,curToken.col,"-- (subscript type is "<<rightExpStatus<<")");
					}
					retVal += ".[:]";
				}
			}
		}
		return retVal;
	} else { // else if this is not an identifier-style Tree node, return a blank string
		return "";
	}
}

// main parsing functions

void shiftToken(Tree *&treeCur, Token &t) {
	Tree *treeToAdd = new Tree(t, NULL, treeCur, NULL, NULL);
	// link right from the current node
	if (treeCur != NULL) {
		*treeCur += treeToAdd;
	}
	// update treeCur to point to the newly shifted node
	treeCur = treeToAdd;
}

void promoteToken(Tree *&treeCur, Token &t) {
	Tree *treeToAdd = new Tree(t, NULL, (treeCur != NULL) ? treeCur->back : NULL, treeCur, (treeCur != NULL) ? treeCur->parent : NULL);
	// relatch on the left
	if (treeCur != NULL && treeCur->back != NULL) {
		*(treeCur->back) += treeToAdd;
		*treeCur -= NULL;
	}
	// link down from parent above
	if (treeCur != NULL && treeCur->parent != NULL) {
		*(treeCur->parent) *= treeToAdd;
	}
	// link up from the current node below
	if (treeCur != NULL) {
		*treeCur &= treeToAdd;
	}
	// set the newly promoted node as the current one
	treeCur = treeToAdd;
}

// treeCur is guaranteed not to be NULL in this case
void shiftPromoteNullToken(Tree *&treeCur, Token &t) {
	Tree *treeToAdd = new Tree(t, NULL, treeCur, NULL, NULL);
	// link in the newly allocated node
	*treeCur += treeToAdd;
	// set treeCur to the newly allocated node
	treeCur = treeToAdd;
}

int parse(vector<Token> *lexeme, Tree *&parseme, unsigned int fileIndex) {

	// initialize local error code
	parserErrorCode = 0;

	// initialize parser structures
#include "../tmp/ruleRhsLengthRaw.h"
#include "../tmp/ruleLhsTokenTypeRaw.h"
#include "../tmp/ruleLhsTokenStringRaw.h"
#include "../tmp/parserNodeRaw.h"

	// iterate through the lexemes and do the actual parsing
	// initialize the current bit of tree that we're examining
	Tree *treeCur = NULL;
	// initialize the state stack and push the initial state onto it
	stack<unsigned int> stateStack;
	stateStack.push(0);

	for(vector<Token>::iterator lexemeIter = lexeme->begin(); lexemeIter != lexeme->end(); lexemeIter++) {

transitionParserState: ;

		// get the current state off the top of the stack
		unsigned int curState = stateStack.top();
		// peek at the next token of input
		Token &t = *lexemeIter;
		// get the transition node for the current state
		ParserNode transition = parserNode[curState][t.tokenType];

		// branch based on the type of action dictated by the transition
		if (transition.action == ACTION_SHIFT) {
			shiftToken(treeCur, t);
			stateStack.push(transition.n);

			VERBOSE( cout << "\t" <<
				COLOREXP(SET_TERM(BRIGHT_CODE AND MAGENTA_CODE)) << "SHIFT" << COLOREXP(SET_TERM(RESET_CODE)) <<
				"\t" << curState << "\t->\t" << transition.n << "\t[" <<
				COLOREXP(SET_TERM(BRIGHT_CODE AND CYAN_CODE)) << tokenType2String(t.tokenType) << COLOREXP(SET_TERM(RESET_CODE)) <<
				"]\n"; )

		} else if (transition.action == ACTION_REDUCE) {
			unsigned int numRhs = ruleRhsLength[transition.n];
			int tokenType = ruleLhsTokenType[transition.n];
			if (numRhs > 1) {
				treeCur = treeCur->goBack(numRhs-1);
			}
			for (unsigned int i=0; i<numRhs; i++) {
				stateStack.pop();
			}
			// create the token that the promoted node will have
			Token t;
			t.tokenType = tokenType;
			t.fileIndex = fileIndex;
			t.row = treeCur != NULL ? treeCur->t.row : 0;
			t.col = treeCur != NULL ? treeCur->t.col : 0;
			// promote the current token, as appropriate
			if (numRhs != 0 || treeCur == NULL) { // if it's not the NULL-shifting promotion case
				promoteToken(treeCur, t);
			} else { // else if it is the NULL-shifting promotion case
				shiftPromoteNullToken(treeCur, t); // note: the above case handles treeCur == NULL
			}
			// take the goto branch of the new transition
			int tempState = stateStack.top();
			stateStack.push(parserNode[tempState][tokenType].n);

			VERBOSE(
				const char *tokenString = ruleLhsTokenString[transition.n];
				cout << "\t" <<
					COLOREXP(SET_TERM(BRIGHT_CODE AND GREEN_CODE)) << "REDUCE" << COLOREXP(SET_TERM(RESET_CODE)) <<
					"\t" << curState << "\t->\t" << stateStack.top() << "\t<" <<
					COLOREXP(SET_TERM(BRIGHT_CODE AND CYAN_CODE)) << tokenString << COLOREXP(SET_TERM(RESET_CODE)) <<
					">\n";
			)

			goto transitionParserState;
		} else if (transition.action == ACTION_ACCEPT) {

			VERBOSE( cout << "\t" <<
				COLOREXP(SET_TERM(BRIGHT_CODE AND WHITE_CODE)) << "ACCEPT" << COLOREXP(SET_TERM(RESET_CODE)) <<
				"\n"; )

			break;
		} else if (transition.action == ACTION_ERROR) {
			string errorString = "syntax error at ";
			if (t.tokenType == TOKEN_CQUOTE || t.tokenType == TOKEN_SQUOTE) {
				errorString += "quoted literal";
			} else {
				errorString += "\'" + t.s + "\'";
			}
			parserError(fileIndex, t.row, t.col, errorString);
			break;
		}
	}

	if (parserErrorCode) { // if there was an error, clean up
		// deallocate the unfinished tree, since there was an error anyway
		delete treeCur;
	} else { // else if there were no errors, log the root parseme into the return slot
		parseme = treeCur;
	}
	// return to the caller
	return parserErrorCode;
}
