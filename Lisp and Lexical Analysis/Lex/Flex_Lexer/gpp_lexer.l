%option caseless

%{
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tokens.h"

bool QUOTE_OP=false;		//for string checking
int TOKEN_ID_HOLDER=0;	//to keep track of token values

void sAuLSV();
void funcTok();
void openParLoop();

typedef struct node{
    char idN[30];
    struct node * child;
}DYNID;

DYNID * defFun;		//to keep defined functions
DYNID * defVal;		//to keep temporary scope defined values	
DYNID * defSVal;	//to keep static defined values (set)

void emptyDYN(DYNID * mem){
	if(mem && mem->child != NULL) emptyDYN(mem->child);
	if(mem) free(mem);
}

void pushFun(){
	DYNID * temp = defFun;
	defFun = (DYNID*)malloc(sizeof(DYNID));
	defFun->child = temp;
	//defFun->idN=yytext;
	strcpy(defFun->idN, yytext);
}

void pushVal(){
	DYNID * temp = defVal;
	defVal = (DYNID*)malloc(sizeof(DYNID));
	defVal->child = temp;
	//defVal->idN=yytext;
	strcpy(defVal->idN, yytext);
}

void pushSVal(){
	DYNID * temp = defSVal;
	defSVal = (DYNID*)malloc(sizeof(DYNID));
	defSVal->child = temp;
	//defSVal->idN=yytext;
	strcpy(defSVal->idN, yytext);
}

void popVal(){
	if(!defVal) return;
	DYNID * temp = defVal;
	defVal = defVal -> child;
	free(temp);
}

bool searchFun(){
	DYNID * ptr=defFun;
	while(ptr != NULL){
		if(strcmp(ptr->idN,yytext)==0) return true;
		ptr = ptr->child;
	}
	return false;
}

bool searchVal(){
	DYNID * ptr=defVal;
	while(ptr != NULL){
		if(strcmp(ptr->idN,yytext)==0) return true;
		ptr = ptr->child;
	}
	ptr=defSVal;
	while(ptr != NULL){
		if(strcmp(ptr->idN,yytext)==0) return true;
		ptr = ptr->child;
	}
	return false;
}

%}

DIGIT		[0-9]+
IDENT		[a-z][a-z0-9]*
STRIN		[^"]

%%

;;.*					return COMMENT;
"and"					return KW_AND;
"or"					return KW_OR;
"not"					return KW_NOT;
\=						return KW_EQUAL;
\<						return KW_LESS;
"for"					return KW_FOR;
"if"					return KW_IF;	/*(if (value) <statement> <optional else statement>)*/
"list"					return KW_LIST;		/*can be used stand-alone*/
"append"				return KW_APPEND;	/*can be used stand-alone*/
"concat"				return KW_CONCAT;
"set"					return KW_SET;
"deffun"				return KW_DEFFUN;	/*It should append a list of defined functions and its values*/
"load"					return KW_LOAD;
"disp"					return KW_DISP;		/*same as print only one input*/
"exit"					return KW_EXIT;
"nil"					return KW_NIL;
"true"					return KW_TRUE;
"false"					return KW_FALSE;
"while"					return KW_WHILE;
\+						return OP_PLUS;
\-						return OP_MINUS;
\/						return OP_DIV;
\*						return OP_MULT;
\(						return OP_OP;
\)						return OP_CP;
"**"					return OP_DBLMULT;
\,						return OP_COMMA;
\"						if(!QUOTE_OP){QUOTE_OP=true; return OP_OC;} else {QUOTE_OP=false; return OP_CC;}
{DIGIT}					return VALUE;
"."{DIGIT}				return VALUE;
{DIGIT}"."{DIGIT}		return VALUE;
{IDENT}					return IDENTIFIER;
[ \t\n]+				;
.						{printf("Unknown character.\n"); exit(0);}

%%

static char * tokenIDName(int id){
	switch (id){
		case 1: return "KW_AND";
		case 2: return "KW_OR";
		case 3: return "KW_NOT";
		case 4: return "KW_EQUAL";
		case 5: return "KW_LESS";
		case 6: return "KW_NIL";
		case 7: return "KW_LIST";
		case 8: return "KW_APPEND";
		case 9: return "KW_CONCAT";
		case 10: return "KW_SET";
		case 11: return "KW_DEFFUN";
		case 12: return "KW_FOR";
		case 13: return "KW_IF";
		case 14: return "KW_EXIT";
		case 15: return "KW_LOAD";
		case 16: return "KW_DISP";
		case 17: return "KW_TRUE";
		case 18: return "KW_FALSE";
		case 19: return "OP_PLUS";
		case 20: return "OP_MINUS";
		case 21: return "OP_DIV";
		case 22: return "OP_MULT";
		case 23: return "OP_OP";
		case 24: return "OP_CP";
		case 25: return "OP_DBLMULT";
		case 26: return "OP_OC";
		case 27: return "OP_CC";
		case 28: return "OP_COMMA";
		case 29: return "COMMENT";
		case 30: return "VALUE";
		case 31: return "IDENTIFIER";
		case 32: return "KW_WHILE";
		default: return "INVALID";
	}
}

void SYN_ERR_USG(int tID){
	printf("SYNTAX_ERROR Wrong usage of function \"%s\"\n", tokenIDName(tID));
	exit(0);
}

bool isVal(){
	if(	TOKEN_ID_HOLDER == VALUE	||
		TOKEN_ID_HOLDER == KW_NIL	||
		TOKEN_ID_HOLDER == KW_TRUE	||
		TOKEN_ID_HOLDER == KW_FALSE	||
		//search name on defVal
		searchVal()
	) return true;
	return false;
}

void stringParse(){
	while((TOKEN_ID_HOLDER = yylex())!=OP_CC)
		if(!TOKEN_ID_HOLDER){
			printf("SYNTAX_ERROR. Unclosed string\n");
			exit(0);}

	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
}

void tokApp(){
	//stand-alone / first is list then unlimited lists or vals
	for(int i=0; i<2 ; i++){
		TOKEN_ID_HOLDER = yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(i==0){
			if (TOKEN_ID_HOLDER==OP_CP) return;
			else if (TOKEN_ID_HOLDER!=OP_OP) SYN_ERR_USG(KW_APPEND);
		}
		if(i==1 && TOKEN_ID_HOLDER!=KW_LIST) SYN_ERR_USG(KW_APPEND);
	}
	sAuLSV();	//token first list
	while((TOKEN_ID_HOLDER = yylex()) && TOKEN_ID_HOLDER != OP_CP){
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER == VALUE);
		else if(TOKEN_ID_HOLDER == OP_OP) openParLoop();/*list or func value*/
		else SYN_ERR_USG(KW_APPEND);
	}
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_APPEND);
}

void tokCon(){
	//stand-alone / first is list then unlimited lists
	for(int i=0; i<2 ; i++){
		TOKEN_ID_HOLDER = yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(i==0){
			if (TOKEN_ID_HOLDER==OP_CP) return;
			else if (TOKEN_ID_HOLDER!=OP_OP) SYN_ERR_USG(KW_CONCAT);
		}
		if(i==1 && TOKEN_ID_HOLDER!=KW_LIST) SYN_ERR_USG(KW_CONCAT);
	}
	sAuLSV();	//token first list
	while((TOKEN_ID_HOLDER = yylex()) && TOKEN_ID_HOLDER != OP_CP){
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER == OP_OP && yylex()==KW_LIST) sAuLSV();	/*only lists*/
		else SYN_ERR_USG(KW_CONCAT);
	}
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_CONCAT);
}

void tokExit(){
	TOKEN_ID_HOLDER = yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER!=OP_CP) SYN_ERR_USG(KW_EXIT);
}

void tokIf(){
	//token val or func
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()){
		if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_IF);
		TOKEN_ID_HOLDER=yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		funcTok();
	}

	//token if statement val or func
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()){
		if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_IF);
		TOKEN_ID_HOLDER=yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		funcTok();
	}
	//optional token else statement or func
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP && !isVal()){
		if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_IF);
		TOKEN_ID_HOLDER=yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		funcTok();
	}
}

void tokFor(){
	//token '('
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_FOR);
	//token identifier
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != IDENTIFIER) SYN_ERR_USG(KW_FOR);
	pushVal();//add token id to temp (can be used in func values)
	//token value
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()) SYN_ERR_USG(KW_FOR);
	//token value
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()) SYN_ERR_USG(KW_FOR);
	//token ')'
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_FOR);
	//token func
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()){
		if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_FOR);
		openParLoop();
	}else{
		TOKEN_ID_HOLDER=yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_FOR);
	}
	
	//free identifier
	popVal();
}

void tokWhile(){
	//token '('
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_FOR);
	//token value
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()) SYN_ERR_USG(KW_FOR);
	//token ')'
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_FOR);
	//token func
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()){
		if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_FOR);
		openParLoop();
	}else{
		TOKEN_ID_HOLDER=yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_FOR);
	}
}

void tokSet(){
	//token identifier
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != IDENTIFIER) SYN_ERR_USG(KW_SET);
	//token value
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()) SYN_ERR_USG(KW_SET);
	//token ')'
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER!=OP_CP) SYN_ERR_USG(KW_SET);
	pushSVal();//add token id to static	(should be added after defined)
}

void tokDeffun(){
	//token identifier
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != IDENTIFIER) SYN_ERR_USG(KW_DEFFUN);
	pushFun();
	//token '('
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER!=OP_OP) SYN_ERR_USG(KW_SET);
	//token identifier
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != IDENTIFIER) SYN_ERR_USG(KW_DEFFUN);
	pushVal();
	//token ')'
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER!=OP_CP) SYN_ERR_USG(KW_DEFFUN);
	//token func
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(!isVal()){
		if(TOKEN_ID_HOLDER != OP_OP) SYN_ERR_USG(KW_FOR);
		openParLoop();
	}else{
		TOKEN_ID_HOLDER=yylex();
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(KW_FOR);
	}
	popVal();
}

void tokLoad(){
	//string start identifier
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_OC) SYN_ERR_USG(KW_LOAD);
	//string token
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	//string end identifier
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CC) SYN_ERR_USG(KW_LOAD);
}

//stand-alone / unlimited val
void sAuV(){
	int tID=TOKEN_ID_HOLDER;
	while((TOKEN_ID_HOLDER = yylex()) && TOKEN_ID_HOLDER != OP_CP){
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER == VALUE);
		else
			SYN_ERR_USG(tID);
	}
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP)
		SYN_ERR_USG(tID);
}

//single list, string or val
void sLSV(){
	int tID=TOKEN_ID_HOLDER;
	TOKEN_ID_HOLDER = yylex();
	if(isVal());
	//else if(TOKEN_ID_HOLDER == OP_OC); //stringParse();
	else if(TOKEN_ID_HOLDER == OP_OP) openParLoop();/*list or func value*/
	else
		SYN_ERR_USG(tID);
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP)
		SYN_ERR_USG(tID);
}

//unlimited val
void uV(){
	int tID=TOKEN_ID_HOLDER;
	if((TOKEN_ID_HOLDER = yylex()) == OP_CP) SYN_ERR_USG(tID);
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	while((TOKEN_ID_HOLDER = yylex()) && TOKEN_ID_HOLDER != OP_CP){
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER == VALUE);
		else SYN_ERR_USG(tID);
	}
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(tID);
}

//stand-alone / unlimited list, string or val
void sAuLSV(){
	int tID=TOKEN_ID_HOLDER;
	while((TOKEN_ID_HOLDER = yylex()) && TOKEN_ID_HOLDER != OP_CP){
		printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
		if(TOKEN_ID_HOLDER == VALUE);
		//else if(TOKEN_ID_HOLDER == OP_OC); //stringParse();
		else if(TOKEN_ID_HOLDER == OP_OP) openParLoop();/*list or func value*/
		else SYN_ERR_USG(tID);
	}
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	if(TOKEN_ID_HOLDER != OP_CP) SYN_ERR_USG(tID);
}

//should be called after a function token is given
void funcTok(){
	switch (TOKEN_ID_HOLDER){
		case KW_AND		:
		case KW_OR		:
		case KW_LIST	:	sAuLSV();	return;
		case KW_EQUAL	:
		case OP_MINUS	:
		case OP_DIV		:	uV();		return;
		case KW_NOT		:
		case KW_DISP	:	sLSV();		return;
		case OP_PLUS	:
		case OP_MULT	:
		case OP_DBLMULT	:	sAuV();		return;
		case KW_APPEND	:	tokApp();	return;
		case KW_CONCAT	:	tokCon();	return;
		case KW_IF		:	tokIf();	return;
		case KW_FOR		:	tokFor();	return;
		case KW_WHILE	:	tokWhile();	return;
		case KW_SET		:	tokSet();	return;
		case KW_DEFFUN	:	tokDeffun();return;
		case KW_EXIT	:	tokExit();	return;
		default			:	printf("SYNTAX_ERROR Token \"%s\" is not valid.\n", yytext); exit(0);
	}
}

//should be called after an OP_OP token is given
void openParLoop(){
	TOKEN_ID_HOLDER=yylex();
	printf("%s\n",tokenIDName(TOKEN_ID_HOLDER));
	switch (TOKEN_ID_HOLDER){
		//empty form is acceptable as nil
		case OP_CP	:	return;
		case VALUE	:
		case KW_NIL	:
		case KW_TRUE:
		case KW_FALSE:
		case OP_OC	:
		case OP_CC	:
		case OP_OP	:	printf("SYNTAX_ERROR S-Expressions should start with a keyword or a function.\n"); exit(0);
		case IDENTIFIER: if(!searchFun())	printf("SYNTAX_ERROR Undefined function.\n"); exit(0);
		default		:	funcTok(); //OP_CP checks are done under sub funcs
	}
}

int main(/*int argc, char *argv[]*/){
	//if(argc != 2) {printf("Input error.\nUsage: ./out <filename>\n"); exit(0);}
	//if(!(yyin = fopen(argv[1],"r"))) {printf("Invalid filename.\n"); exit(0);}
	//scanf("%s",yyin);

	while(TOKEN_ID_HOLDER = yylex()) {
		printf("%s\n", tokenIDName(TOKEN_ID_HOLDER));
		switch (TOKEN_ID_HOLDER){
			case VALUE:
			case COMMENT:
			case KW_NIL:
			case KW_TRUE:
			case KW_FALSE: break;
			case OP_OP: openParLoop(); break;
			case OP_OC:	stringParse(); break;
			case OP_CP:
				printf("SYNTAX_ERROR. Expected \' \( \' \n");
				exit(0);
			default: 
				printf("SYNTAX_ERROR. Invalid function or keyword usage\n");
				exit(0);
		}
	}

	if(QUOTE_OP){
		printf("SYNTAX_ERROR. Unclosed string\n");exit(0);
	}
	
	emptyDYN(defFun); emptyDYN(defVal); emptyDYN(defSVal);
	
	return 0;
}