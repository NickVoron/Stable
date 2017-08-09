%option 8bit
%option noyywrap
%option noyylineno
%option caseful
%option c++
%option yyclass="ObjectParser::Lexer"
%option pointer

%{
	#include "lexer.h"
	#include "parser.h"
	#include "../compiler/compiler.h"
	
%}

%x COMMENT
%x EXTERNAL_EXTENSION

%{
	#define YY_USER_ACTION  yylloc->columns (yyleng);

%}

%%

%{
	yylloc->step ();
%}

class		{	return Parser::token::CLASS;		}
interface	{	return Parser::token::INTERFACE;	}
this		{	return Parser::token::THISS;		}
collection	{	return Parser::token::COLLECTION;	}
[#]include  { 	return Parser::token::INCLUDE;		}
var			{	return Parser::token::VAR;			}
property	{	return Parser::token::PROPERTY;		}
array		{	return Parser::token::ARRAY;		}
noinstance	{	return Parser::token::NOINSTANCE;	}
component	{	return Parser::token::COMPONENT;	}
true		{ 	return Parser::token::true_key;		}
false		{	return Parser::token::false_key;	}
iterator	{	return Parser::token::ITERATOR;		}
external	{	return Parser::token::EXTERNAL;		}
private		{	return Parser::token::PRIVATE;		}
mix			{	return Parser::token::MIX;		}
aggregate	{	return Parser::token::AGGREGATE;		}

"/*"			{	BEGIN COMMENT; }
<COMMENT>"*/"	{	BEGIN 0; }
<COMMENT>.|\n {};

"{{"						{ BEGIN EXTERNAL_EXTENSION; }
<EXTERNAL_EXTENSION>"}}"	{ BEGIN 0; }
<EXTERNAL_EXTENSION>.|\n	{};


\"[^\n"]*\"	{
				size_t len = strlen(yytext);
				strncpy(yylval->name, yytext+1, len-2);
				yylval->name[len-2] = '\0';
				return Parser::token::str;
			}
	

[a-zA-Z][a-zA-Z0-9_]*	{ strcpy(yylval->name, yytext); return Parser::token::literal; }
[-]*[0-9]+				{ yylval->intVal = atoi(yytext);	return Parser::token::digit; }
[-]*[0-9]*[.][0-9]+f	{ yylval->fltVal = (float) myatof(yytext);	return Parser::token::flt; }
[-]*[0-9]*[.][0-9]+		{ yylval->fltVal = (float) myatof(yytext); return Parser::token::flt; }

	
[&]		return Parser::token::AND;
[|]		return Parser::token::OR;
[=][=]	return Parser::token::EQUAL;
[!][=]	return Parser::token::NOT_EQUAL;
[<][=]	return Parser::token::LESS_EQUAL;
[>][=]	return Parser::token::MORE_EQUAL;
[+]		return Parser::token::PLUS;
[-]		return Parser::token::MINUS;
[%]		return Parser::token::MOD;
[/]		return Parser::token::DIV;
[*]		return Parser::token::MUL;
[!]		return Parser::token::NOT;
[?]		return Parser::token::QUERY;


[#<>{},=;:().\]\[\?]     { return *yytext; }

[ \t\r]+	yylloc->step ();       /* skip whitespace */
[\n]+  yylloc->lines (yyleng); yylloc->step ();

.			{	printError( yytext, *yylloc); }

"//".*		{	}

%%

//[<]		return Parser::token::LESS;
//[>]		return Parser::token::MORE;
//[:]		return Parser::token::COLON;
//[\^]		return Parser::token::XOR;