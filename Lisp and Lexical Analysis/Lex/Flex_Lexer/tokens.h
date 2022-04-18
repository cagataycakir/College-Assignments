#ifndef TOKEN_VAL_H
#define TOKEN_VAL_H

#include <stdio.h>

/*Keywords*/
#define KW_AND 1
#define KW_OR 2
#define KW_NOT 3
#define KW_EQUAL 4
#define KW_LESS 5
#define KW_NIL 6
#define KW_LIST 7
#define KW_APPEND 8
#define KW_CONCAT 9
#define KW_SET 10
#define KW_DEFFUN 11
#define KW_FOR 12
#define KW_IF 13
#define KW_EXIT 14
#define KW_LOAD 15
#define KW_DISP 16
#define KW_TRUE 17
#define KW_FALSE 18
/*Operators*/
#define OP_PLUS 19
#define OP_MINUS 20
#define OP_DIV 21
#define OP_MULT 22
#define OP_OP 23
#define OP_CP 24
#define OP_DBLMULT 25
#define OP_OC 26
#define OP_CC 27
#define OP_COMMA 28
/*Terminals / Blocks*/
#define COMMENT 29
#define VALUE 30
#define IDENTIFIER 31
#define KW_WHILE 32

#endif

/*** logical ***/
/*can only be put at start of an s-expression*/

//"and"		stand-alone / unlimited list, string or val
//"or"		stand-alone / unlimited list, string or val
//"not"		single list, string or val
//"="		unlimited val
//"<"		unlimited val

/***functional***/
/*can only be put at start of an s-expression*/

//"for"		(for (identifier value value) function)
//"while"	(while value function)
//"if"		(if value function <optional else function>)
//"list"	stand-alone / unlimited list, string or val
//"append"	stand-alone / first is list then unlimited lists or vals
//"concat"	stand-alone / first is list then unlimited lists
//"set"		(set identifier value)
//"deffun"	(deffun identifier (identifier) function )
//"load"	(load string)
//"disp"	single list, string or val

/***exit***/
/*exit is a special atom that can only be used like this -> (exit)
can be put in a list or a function
the program gets terminated when reaches the form*/
//"exit"	(exit)

/***constants***/
/*cannot be put at start of an s-expression*/
//"nil"
//"true"
//"false"

/*** arithmetical ***/
/*can only be put at start of an s-expression*/
//"+"	stand-alone / unlimited val
//"-"	unlimited val
//"/"	unlimited val
//"*"	stand-alone / unlimited val
//"**"	stand-alone / unlimited val

/*** blocks ***/
/*cannot be put at start of an s-expression
identifiers can be if it they are user defined functions
but not function input indentifiers*/
//"("
//")"
//","
//\"
//VALUE
//IDENTIFIER