// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#line 2 "parser/lexer.cpp"




#define FLEX_SCANNER
#define YY_FLEX_MAJOR_VERSION 2
#define YY_FLEX_MINOR_VERSION 5




#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif


#ifdef __cplusplus

#include <stdlib.h>


#include <iostream>
using namespace std;


#define YY_USE_PROTOS


#define YY_USE_CONST

#else	

#if __STDC__

#define YY_USE_PROTOS
#define YY_USE_CONST

#endif	
#endif	

#ifdef __TURBOC__
 #pragma warn -rch
 #pragma warn -use
#include <io.h>
#include <stdlib.h>
#define YY_USE_CONST
#define YY_USE_PROTOS
#endif

#ifdef YY_USE_CONST
#define yyconst const
#else
#define yyconst
#endif


#ifdef YY_USE_PROTOS
#define YY_PROTO(proto) proto
#else
#define YY_PROTO(proto) ()
#endif


#define YY_NULL 0


#define YY_SC_TO_UI(c) ((unsigned int) (unsigned char) c)


#define BEGIN yy_start = 1 + 2 *


#define YY_START ((yy_start - 1) / 2)
#define YYSTATE YY_START


#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)


#define YY_NEW_FILE yyrestart( yyin )

#define YY_END_OF_BUFFER_CHAR 0


#define YY_BUF_SIZE 16384

typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern int yyleng;

#define EOB_ACT_CONTINUE_SCAN 0
#define EOB_ACT_END_OF_FILE 1
#define EOB_ACT_LAST_MATCH 2





#define yyless(n) \
	do \
		{ \
		 \
		*yy_cp = yy_hold_char; \
		YY_RESTORE_YY_MORE_OFFSET \
		yy_c_buf_p = yy_cp = yy_bp + n - YY_MORE_ADJ; \
		YY_DO_BEFORE_ACTION;  \
		} \
	while ( 0 )

#define unput(c) yyunput( c, yytext_ptr )


typedef unsigned int yy_size_t;


struct yy_buffer_state
	{
	std::istream* yy_input_file;

	char *yy_ch_buf;		
	char *yy_buf_pos;		

	
	yy_size_t yy_buf_size;

	
	int yy_n_chars;

	
	int yy_is_our_buffer;

	
	int yy_is_interactive;

	
	int yy_at_bol;

	
	int yy_fill_buffer;

	int yy_buffer_status;
#define YY_BUFFER_NEW 0
#define YY_BUFFER_NORMAL 1
	
#define YY_BUFFER_EOF_PENDING 2
	};



#define YY_CURRENT_BUFFER yy_current_buffer



static void *yy_flex_alloc YY_PROTO(( yy_size_t ));
static void *yy_flex_realloc YY_PROTO(( void *, yy_size_t ));
static void yy_flex_free YY_PROTO(( void * ));

#define yy_new_buffer yy_create_buffer

#define yy_set_interactive(is_interactive) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_is_interactive = is_interactive; \
	}

#define yy_set_bol(at_bol) \
	{ \
	if ( ! yy_current_buffer ) \
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE ); \
	yy_current_buffer->yy_at_bol = at_bol; \
	}

#define YY_AT_BOL() (yy_current_buffer->yy_at_bol)


#define yywrap() 1
#define YY_SKIP_YYWRAP
typedef unsigned char YY_CHAR;
#define yytext_ptr yytext
#define YY_INTERACTIVE

#include <FlexLexer.h>
int yyFlexLexer::yylex()
	{
	LexerError( "yyFlexLexer::yylex invoked but %option yyclass used" );
	return 0;
	}

#define YY_DECL int ObjectParser::Lexer::yylex()



#define YY_DO_BEFORE_ACTION \
	yytext_ptr = yy_bp; \
	yyleng = (int) (yy_cp - yy_bp); \
	yy_hold_char = *yy_cp; \
	*yy_cp = '\0'; \
	yy_c_buf_p = yy_cp;

#define YY_NUM_RULES 47
#define YY_END_OF_BUFFER 48
static yyconst short int yy_accept[161] =
    {   0,
        0,    0,    0,    0,    0,    0,   48,   45,   43,   44,
       40,   45,   42,   37,   29,   42,   39,   35,   36,   42,
       38,   26,   42,   42,   42,   41,   25,   25,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   42,   30,   20,
       20,   23,   23,   43,   44,   32,    0,   24,    0,    0,
        0,   26,   28,   18,   46,   33,   31,   34,   25,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   25,   25,
       25,   25,   25,   21,   19,   22,    0,   27,   46,   25,
       25,   25,   25,   25,   25,   25,   25,   25,   16,   25,
       25,   25,   25,   25,    6,    0,   25,   25,   25,   25,

       25,   25,   25,   25,   25,   25,   25,   25,    3,   11,
        0,   25,    8,    1,   25,   25,   25,   12,   25,   25,
       25,   25,   25,    0,   25,   25,   25,   25,   25,   25,
       25,   25,   25,    0,   25,   25,   25,   25,   25,   25,
       25,   15,   25,    5,   25,   25,   25,   14,   25,   13,
       25,    7,   17,   25,   10,    2,   25,    4,    9,    0
    } ;

static yyconst int yy_ec[256] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    2,    3,
        1,    1,    2,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    2,    4,    5,    6,    1,    7,    8,    1,    9,
        9,   10,   11,    9,   12,   13,   14,   15,   15,   15,
       15,   15,   15,   15,   15,   15,   15,    9,    9,   16,
       17,   18,   19,    1,   20,   20,   20,   20,   20,   20,
       20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
       20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
        9,    1,    9,    1,   21,    1,   22,   20,   23,   24,

       25,   26,   27,   28,   29,   20,   20,   30,   31,   32,
       33,   34,   20,   35,   36,   37,   38,   39,   20,   40,
       41,   20,   42,   43,   44,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,

        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1
    } ;

static yyconst int yy_meta[45] =
    {   0,
        1,    1,    2,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    3,    1,    1,    1,    1,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
        3,    1,    1,    1
    } ;

static yyconst short int yy_base[166] =
    {   0,
        0,    0,  175,  174,  139,  138,  181,  186,  178,  176,
      161,  172,  147,  186,  186,  186,  186,  186,   33,  160,
       37,   37,  157,  156,  155,  186,    0,   22,   23,  131,
      148,   22,  140,  135,  132,   27,  144,  123,  186,  186,
      150,  186,  119,  160,  158,  186,  155,  186,  127,   48,
      143,   51,   43,  186,    0,  186,  186,  186,    0,  130,
      121,  133,   37,  117,  123,  115,  126,  110,  120,   41,
      119,  109,  111,  186,  186,  186,  122,  186,    0,  109,
      121,  106,  111,  106,  114,  102,  112,  101,    0,  103,
       95,   99,   96,  106,    0,  100,  104,   87,   91,  101,

       92,   89,   98,   87,   99,   84,   97,   93,    0,    0,
       79,   89,    0,    0,   92,   82,   81,    0,   86,   74,
       73,   72,   73,   83,   84,   68,   79,   81,   80,   68,
       78,   74,   61,   72,   59,   66,   62,   63,   69,   56,
       58,    0,   48,  186,   63,   54,   48,    0,   57,    0,
       50,    0,    0,   40,    0,    0,   46,    0,    0,  186,
       74,   77,   80,   62,   83
    } ;

static yyconst short int yy_def[166] =
    {   0,
      160,    1,  161,  161,  162,  162,  160,  160,  160,  160,
      160,  163,  160,  160,  160,  160,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  164,  164,  164,  164,
      164,  164,  164,  164,  164,  164,  164,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  163,  160,  160,  160,
      160,  160,  160,  160,  165,  160,  160,  160,  164,  164,
      164,  164,  164,  164,  164,  164,  164,  164,  164,  164,
      164,  164,  164,  160,  160,  160,  160,  160,  165,  164,
      164,  164,  164,  164,  164,  164,  164,  164,  164,  164,
      164,  164,  164,  164,  164,  160,  164,  164,  164,  164,

      164,  164,  164,  164,  164,  164,  164,  164,  164,  164,
      160,  164,  164,  164,  164,  164,  164,  164,  164,  164,
      164,  164,  164,  160,  164,  164,  164,  164,  164,  164,
      164,  164,  164,  160,  164,  164,  164,  164,  164,  164,
      164,  164,  164,  160,  164,  164,  164,  164,  164,  164,
      164,  164,  164,  164,  164,  164,  164,  164,  164,    0,
      160,  160,  160,  160,  160
    } ;

static yyconst short int yy_nxt[231] =
    {   0,
        8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
       18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
        8,   28,   29,   27,   30,   31,   27,   27,   32,   27,
       33,   34,   27,   35,   27,   27,   36,   27,   37,   27,
       27,   38,   39,   16,   50,   51,   54,   52,   60,   51,
       55,   52,   62,   66,   71,   63,   61,   53,   67,   50,
       51,   72,   52,   51,   59,   52,   83,   84,   78,   91,
      159,  158,  157,   92,   40,   40,   40,   42,   42,   42,
       47,  156,   47,   79,  155,   79,  154,  153,  152,  151,
      150,  149,  148,  147,  146,  145,  144,  143,  142,  141,

      140,  139,  138,  137,  136,  135,  134,  133,  132,  131,
      130,  129,  128,  127,  126,  125,  124,  123,  122,  121,
      120,  119,  118,  117,  116,  115,  114,  113,  112,  111,
      110,  109,  108,  107,  106,  105,  104,  103,  102,  101,
      100,   99,   98,   97,   96,   95,   94,   93,   90,   89,
       88,   87,   86,   85,   82,   81,   80,   53,   77,   48,
       45,   44,   76,   75,   74,   73,   70,   69,   68,   65,
       64,   58,   57,   56,   53,   49,   48,   46,   45,   44,
      160,   43,   43,   41,   41,    7,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  160,  160,  160,  160,

      160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  160,  160,  160,  160
    } ;

static yyconst short int yy_chk[231] =
    {   0,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
        1,    1,    1,    1,   19,   19,   21,   19,   28,   22,
       21,   22,   29,   32,   36,   29,   28,   53,   32,   50,
       50,   36,   50,   52,  164,   52,   63,   63,   53,   70,
      157,  154,  151,   70,  161,  161,  161,  162,  162,  162,
      163,  149,  163,  165,  147,  165,  146,  145,  143,  141,
      140,  139,  138,  137,  136,  135,  134,  133,  132,  131,

      130,  129,  128,  127,  126,  125,  124,  123,  122,  121,
      120,  119,  117,  116,  115,  112,  111,  108,  107,  106,
      105,  104,  103,  102,  101,  100,   99,   98,   97,   96,
       94,   93,   92,   91,   90,   88,   87,   86,   85,   84,
       83,   82,   81,   80,   77,   73,   72,   71,   69,   68,
       67,   66,   65,   64,   62,   61,   60,   51,   49,   47,
       45,   44,   43,   41,   38,   37,   35,   34,   33,   31,
       30,   25,   24,   23,   20,   13,   12,   11,   10,    9,
        7,    6,    5,    4,    3,  160,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  160,  160,  160,  160,

      160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  160,  160,  160,  160,
      160,  160,  160,  160,  160,  160,  160,  160,  160,  160
    } ;


#define REJECT reject_used_but_not_detected
#define yymore() yymore_used_but_not_detected
#define YY_MORE_ADJ 0
#define YY_RESTORE_YY_MORE_OFFSET
#line 1 "parser/lexer.lex"
#define INITIAL 0
#line 10 "parser/lexer.lex"
	#include "lexer.h"
	#include "parser.h"
	#include "../compiler/compiler.h"
	
#define COMMENT 1

#define EXTERNAL_EXTENSION 2

#line 20 "parser/lexer.lex"
	#define YY_USER_ACTION  yylloc->columns (yyleng);

#line 449 "parser/lexer.cpp"



#ifndef YY_SKIP_YYWRAP
#ifdef __cplusplus
extern "C" int yywrap YY_PROTO(( void ));
#else
extern int yywrap YY_PROTO(( void ));
#endif
#endif


#ifndef yytext_ptr
static void yy_flex_strncpy YY_PROTO(( char *, yyconst char *, int ));
#endif

#ifdef YY_NEED_STRLEN
static int yy_flex_strlen YY_PROTO(( yyconst char * ));
#endif

#ifndef YY_NO_INPUT
#endif

#if YY_STACK_USED
static int yy_start_stack_ptr = 0;
static int yy_start_stack_depth = 0;
static int *yy_start_stack = 0;
#ifndef YY_NO_PUSH_STATE
static void yy_push_state YY_PROTO(( int new_state ));
#endif
#ifndef YY_NO_POP_STATE
static void yy_pop_state YY_PROTO(( void ));
#endif
#ifndef YY_NO_TOP_STATE
static int yy_top_state YY_PROTO(( void ));
#endif

#else
#define YY_NO_PUSH_STATE 1
#define YY_NO_POP_STATE 1
#define YY_NO_TOP_STATE 1
#endif

#ifdef YY_MALLOC_DECL
YY_MALLOC_DECL
#else
#if __STDC__
#ifndef __cplusplus
#include <stdlib.h>
#endif
#else

#endif
#endif


#ifndef YY_READ_BUF_SIZE
#define YY_READ_BUF_SIZE 8192
#endif



#ifndef ECHO
#define ECHO LexerOutput( yytext, yyleng )
#endif


#ifndef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = LexerInput( (char *) buf, max_size )) < 0 ) \
		YY_FATAL_ERROR( "input in flex scanner failed" );
#endif


#ifndef yyterminate
#define yyterminate() return YY_NULL
#endif


#ifndef YY_START_STACK_INCR
#define YY_START_STACK_INCR 25
#endif


#ifndef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) LexerError( msg )
#endif


#ifndef YY_DECL
#define YY_DECL int yyFlexLexer::yylex()
#endif


#ifndef YY_USER_ACTION
#define YY_USER_ACTION
#endif


#ifndef YY_BREAK
#define YY_BREAK break;
#endif

#define YY_RULE_SETUP \
	YY_USER_ACTION

YY_DECL
	{
	register yy_state_type yy_current_state;
	register char *yy_cp, *yy_bp;
	register int yy_act;

#line 24 "parser/lexer.lex"



	yylloc->step ();


#line 583 "parser/lexer.cpp"

	if ( yy_init )
		{
		yy_init = 0;

#ifdef YY_USER_INIT
		YY_USER_INIT;
#endif

		if ( ! yy_start )
			yy_start = 1;	

		if ( ! yyin )
			yyin = &cin;

		if ( ! yyout )
			yyout = &cout;

		if ( ! yy_current_buffer )
			yy_current_buffer =
				yy_create_buffer( yyin, YY_BUF_SIZE );

		yy_load_buffer_state();
		}

	while ( 1 )		
		{
		yy_cp = yy_c_buf_p;

		
		*yy_cp = yy_hold_char;

		
		yy_bp = yy_cp;

		yy_current_state = yy_start;
yy_match:
		do
			{
			register YY_CHAR yy_c = yy_ec[YY_SC_TO_UI(*yy_cp)];
			if ( yy_accept[yy_current_state] )
				{
				yy_last_accepting_state = yy_current_state;
				yy_last_accepting_cpos = yy_cp;
				}
			while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
				{
				yy_current_state = (int) yy_def[yy_current_state];
				if ( yy_current_state >= 161 )
					yy_c = yy_meta[(unsigned int) yy_c];
				}
			yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
			++yy_cp;
			}
		while ( yy_base[yy_current_state] != 186 );

yy_find_action:
		yy_act = yy_accept[yy_current_state];
		if ( yy_act == 0 )
			{ 
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			yy_act = yy_accept[yy_current_state];
			}

		YY_DO_BEFORE_ACTION;


do_action:	


		switch ( yy_act )
	{ 
			case 0: 
			
			*yy_cp = yy_hold_char;
			yy_cp = yy_last_accepting_cpos;
			yy_current_state = yy_last_accepting_state;
			goto yy_find_action;

case 1:
YY_RULE_SETUP
#line 30 "parser/lexer.lex"
{	return Parser::token::CLASS;		}
	YY_BREAK
case 2:
YY_RULE_SETUP
#line 31 "parser/lexer.lex"
{	return Parser::token::INTERFACE;	}
	YY_BREAK
case 3:
YY_RULE_SETUP
#line 32 "parser/lexer.lex"
{	return Parser::token::THISS;		}
	YY_BREAK
case 4:
YY_RULE_SETUP
#line 33 "parser/lexer.lex"
{	return Parser::token::COLLECTION;	}
	YY_BREAK
case 5:
YY_RULE_SETUP
#line 34 "parser/lexer.lex"
{ 	return Parser::token::INCLUDE;		}
	YY_BREAK
case 6:
YY_RULE_SETUP
#line 35 "parser/lexer.lex"
{	return Parser::token::VAR;			}
	YY_BREAK
case 7:
YY_RULE_SETUP
#line 36 "parser/lexer.lex"
{	return Parser::token::PROPERTY;		}
	YY_BREAK
case 8:
YY_RULE_SETUP
#line 37 "parser/lexer.lex"
{	return Parser::token::ARRAY;		}
	YY_BREAK
case 9:
YY_RULE_SETUP
#line 38 "parser/lexer.lex"
{	return Parser::token::NOINSTANCE;	}
	YY_BREAK
case 10:
YY_RULE_SETUP
#line 39 "parser/lexer.lex"
{	return Parser::token::COMPONENT;	}
	YY_BREAK
case 11:
YY_RULE_SETUP
#line 40 "parser/lexer.lex"
{ 	return Parser::token::true_key;		}
	YY_BREAK
case 12:
YY_RULE_SETUP
#line 41 "parser/lexer.lex"
{	return Parser::token::false_key;	}
	YY_BREAK
case 13:
YY_RULE_SETUP
#line 42 "parser/lexer.lex"
{	return Parser::token::ITERATOR;		}
	YY_BREAK
case 14:
YY_RULE_SETUP
#line 43 "parser/lexer.lex"
{	return Parser::token::EXTERNAL;		}
	YY_BREAK
case 15:
YY_RULE_SETUP
#line 44 "parser/lexer.lex"
{	return Parser::token::PRIVATE;		}
	YY_BREAK
case 16:
YY_RULE_SETUP
#line 45 "parser/lexer.lex"
{	return Parser::token::MIX;		}
	YY_BREAK
case 17:
YY_RULE_SETUP
#line 46 "parser/lexer.lex"
{	return Parser::token::AGGREGATE;		}
	YY_BREAK
case 18:
YY_RULE_SETUP
#line 48 "parser/lexer.lex"
{	BEGIN COMMENT; }
	YY_BREAK
case 19:
YY_RULE_SETUP
#line 49 "parser/lexer.lex"
{	BEGIN 0; }
	YY_BREAK
case 20:
YY_RULE_SETUP
#line 50 "parser/lexer.lex"
{};
	YY_BREAK
case 21:
YY_RULE_SETUP
#line 52 "parser/lexer.lex"
{ BEGIN EXTERNAL_EXTENSION; }
	YY_BREAK
case 22:
YY_RULE_SETUP
#line 53 "parser/lexer.lex"
{ BEGIN 0; }
	YY_BREAK
case 23:
YY_RULE_SETUP
#line 54 "parser/lexer.lex"
{};
	YY_BREAK
case 24:
YY_RULE_SETUP
#line 57 "parser/lexer.lex"
{
				size_t len = strlen(yytext);
				strncpy(yylval->name, yytext+1, len-2);
				yylval->name[len-2] = '\0';
				return Parser::token::str;
			}
	YY_BREAK
case 25:
YY_RULE_SETUP
#line 65 "parser/lexer.lex"
{ strcpy(yylval->name, yytext); return Parser::token::literal; }
	YY_BREAK
case 26:
YY_RULE_SETUP
#line 66 "parser/lexer.lex"
{ yylval->intVal = atoi(yytext);	return Parser::token::digit; }
	YY_BREAK
case 27:
YY_RULE_SETUP
#line 67 "parser/lexer.lex"
{ yylval->fltVal = (float) myatof(yytext);	return Parser::token::flt; }
	YY_BREAK
case 28:
YY_RULE_SETUP
#line 68 "parser/lexer.lex"
{ yylval->fltVal = (float) myatof(yytext); return Parser::token::flt; }
	YY_BREAK
case 29:
YY_RULE_SETUP
#line 71 "parser/lexer.lex"
return Parser::token::AND;
	YY_BREAK
case 30:
YY_RULE_SETUP
#line 72 "parser/lexer.lex"
return Parser::token::OR;
	YY_BREAK
case 31:
YY_RULE_SETUP
#line 73 "parser/lexer.lex"
return Parser::token::EQUAL;
	YY_BREAK
case 32:
YY_RULE_SETUP
#line 74 "parser/lexer.lex"
return Parser::token::NOT_EQUAL;
	YY_BREAK
case 33:
YY_RULE_SETUP
#line 75 "parser/lexer.lex"
return Parser::token::LESS_EQUAL;
	YY_BREAK
case 34:
YY_RULE_SETUP
#line 76 "parser/lexer.lex"
return Parser::token::MORE_EQUAL;
	YY_BREAK
case 35:
YY_RULE_SETUP
#line 77 "parser/lexer.lex"
return Parser::token::PLUS;
	YY_BREAK
case 36:
YY_RULE_SETUP
#line 78 "parser/lexer.lex"
return Parser::token::MINUS;
	YY_BREAK
case 37:
YY_RULE_SETUP
#line 79 "parser/lexer.lex"
return Parser::token::MOD;
	YY_BREAK
case 38:
YY_RULE_SETUP
#line 80 "parser/lexer.lex"
return Parser::token::DIV;
	YY_BREAK
case 39:
YY_RULE_SETUP
#line 81 "parser/lexer.lex"
return Parser::token::MUL;
	YY_BREAK
case 40:
YY_RULE_SETUP
#line 82 "parser/lexer.lex"
return Parser::token::NOT;
	YY_BREAK
case 41:
YY_RULE_SETUP
#line 83 "parser/lexer.lex"
return Parser::token::QUERY;
	YY_BREAK
case 42:
YY_RULE_SETUP
#line 86 "parser/lexer.lex"
{ return *yytext; }
	YY_BREAK
case 43:
YY_RULE_SETUP
#line 88 "parser/lexer.lex"
yylloc->step ();       
	YY_BREAK
case 44:
YY_RULE_SETUP
#line 89 "parser/lexer.lex"
yylloc->lines (yyleng); yylloc->step ();
	YY_BREAK
case 45:
YY_RULE_SETUP
#line 91 "parser/lexer.lex"
{	printError( yytext, *yylloc); }
	YY_BREAK
case 46:
YY_RULE_SETUP
#line 93 "parser/lexer.lex"
{	}
	YY_BREAK
case 47:
YY_RULE_SETUP
#line 95 "parser/lexer.lex"
ECHO;
	YY_BREAK
#line 906 "parser/lexer.cpp"
case YY_STATE_EOF(INITIAL):
case YY_STATE_EOF(COMMENT):
case YY_STATE_EOF(EXTERNAL_EXTENSION):
	yyterminate();

	case YY_END_OF_BUFFER:
		{
		
		int yy_amount_of_matched_text = (int) (yy_cp - yytext_ptr) - 1;

		
		*yy_cp = yy_hold_char;
		YY_RESTORE_YY_MORE_OFFSET

		if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_NEW )
			{
			
			yy_n_chars = yy_current_buffer->yy_n_chars;
			yy_current_buffer->yy_input_file = yyin;
			yy_current_buffer->yy_buffer_status = YY_BUFFER_NORMAL;
			}

		
		if ( yy_c_buf_p <= &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			{ 
			yy_state_type yy_next_state;

			yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text;

			yy_current_state = yy_get_previous_state();

			

			yy_next_state = yy_try_NUL_trans( yy_current_state );

			yy_bp = yytext_ptr + YY_MORE_ADJ;

			if ( yy_next_state )
				{
				
				yy_cp = ++yy_c_buf_p;
				yy_current_state = yy_next_state;
				goto yy_match;
				}

			else
				{
				yy_cp = yy_c_buf_p;
				goto yy_find_action;
				}
			}

		else switch ( yy_get_next_buffer() )
			{
			case EOB_ACT_END_OF_FILE:
				{
				yy_did_buffer_switch_on_eof = 0;

				if ( yywrap() )
					{
					
					yy_c_buf_p = yytext_ptr + YY_MORE_ADJ;

					yy_act = YY_STATE_EOF(YY_START);
					goto do_action;
					}

				else
					{
					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
					}
				break;
				}

			case EOB_ACT_CONTINUE_SCAN:
				yy_c_buf_p =
					yytext_ptr + yy_amount_of_matched_text;

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_match;

			case EOB_ACT_LAST_MATCH:
				yy_c_buf_p =
				&yy_current_buffer->yy_ch_buf[yy_n_chars];

				yy_current_state = yy_get_previous_state();

				yy_cp = yy_c_buf_p;
				yy_bp = yytext_ptr + YY_MORE_ADJ;
				goto yy_find_action;
			}
		break;
		}

	default:
		YY_FATAL_ERROR(
			"fatal flex scanner internal error--no action found" );
	} 
		} 
	} 

yyFlexLexer::yyFlexLexer( istream* arg_yyin, ostream* arg_yyout )
	{
	yyin = arg_yyin;
	yyout = arg_yyout;
	yy_c_buf_p = 0;
	yy_init = 1;
	yy_start = 0;
	yy_flex_debug = 0;
	yylineno = 1;	

	yy_did_buffer_switch_on_eof = 0;

	yy_looking_for_trail_begin = 0;
	yy_more_flag = 0;
	yy_more_len = 0;
	yy_more_offset = yy_prev_more_offset = 0;

	yy_start_stack_ptr = yy_start_stack_depth = 0;
	yy_start_stack = 0;

	yy_current_buffer = 0;

#ifdef YY_USES_REJECT
	yy_state_buf = new yy_state_type[YY_BUF_SIZE + 2];
#else
	yy_state_buf = 0;
#endif
	}

yyFlexLexer::~yyFlexLexer()
	{
	delete yy_state_buf;
	yy_delete_buffer( yy_current_buffer );
	}

void yyFlexLexer::switch_streams( istream* new_in, ostream* new_out )
	{
	if ( new_in )
		{
		yy_delete_buffer( yy_current_buffer );
		yy_switch_to_buffer( yy_create_buffer( new_in, YY_BUF_SIZE ) );
		}

	if ( new_out )
		yyout = new_out;
	}

#ifdef YY_INTERACTIVE
int yyFlexLexer::LexerInput( char* buf, int  )
#else
int yyFlexLexer::LexerInput( char* buf, int max_size )
#endif
	{
	if ( yyin->eof() || yyin->fail() )
		return 0;

#ifdef YY_INTERACTIVE
	yyin->get( buf[0] );

	if ( yyin->eof() )
		return 0;

	if ( yyin->bad() )
		return -1;

	return 1;

#else
	(void) yyin->read( buf, max_size );

	if ( yyin->bad() )
		return -1;
	else
		return yyin->gcount();
#endif
	}

void yyFlexLexer::LexerOutput( const char* buf, int size )
	{
	(void) yyout->write( buf, size );
	}



int yyFlexLexer::yy_get_next_buffer()
	{
	register char *dest = yy_current_buffer->yy_ch_buf;
	register char *source = yytext_ptr;
	register int number_to_move, i;
	int ret_val;

	if ( yy_c_buf_p > &yy_current_buffer->yy_ch_buf[yy_n_chars + 1] )
		YY_FATAL_ERROR(
		"fatal flex scanner internal error--end of buffer missed" );

	if ( yy_current_buffer->yy_fill_buffer == 0 )
		{ 
		if ( yy_c_buf_p - yytext_ptr - YY_MORE_ADJ == 1 )
			{
			
			return EOB_ACT_END_OF_FILE;
			}

		else
			{
			
			return EOB_ACT_LAST_MATCH;
			}
		}

	

	
	number_to_move = (int) (yy_c_buf_p - yytext_ptr) - 1;

	for ( i = 0; i < number_to_move; ++i )
		*(dest++) = *(source++);

	if ( yy_current_buffer->yy_buffer_status == YY_BUFFER_EOF_PENDING )
		
		yy_current_buffer->yy_n_chars = yy_n_chars = 0;

	else
		{
		int num_to_read =
			yy_current_buffer->yy_buf_size - number_to_move - 1;

		while ( num_to_read <= 0 )
			{ 
#ifdef YY_USES_REJECT
			YY_FATAL_ERROR(
"input buffer overflow, can't enlarge buffer because scanner uses REJECT" );
#else

			
			YY_BUFFER_STATE b = yy_current_buffer;

			int yy_c_buf_p_offset =
				(int) (yy_c_buf_p - b->yy_ch_buf);

			if ( b->yy_is_our_buffer )
				{
				int new_size = b->yy_buf_size * 2;

				if ( new_size <= 0 )
					b->yy_buf_size += b->yy_buf_size / 8;
				else
					b->yy_buf_size *= 2;

				b->yy_ch_buf = (char *)
					
					yy_flex_realloc( (void *) b->yy_ch_buf,
							 b->yy_buf_size + 2 );
				}
			else
				
				b->yy_ch_buf = 0;

			if ( ! b->yy_ch_buf )
				YY_FATAL_ERROR(
				"fatal error - scanner input buffer overflow" );

			yy_c_buf_p = &b->yy_ch_buf[yy_c_buf_p_offset];

			num_to_read = yy_current_buffer->yy_buf_size -
						number_to_move - 1;
#endif
			}

		if ( num_to_read > YY_READ_BUF_SIZE )
			num_to_read = YY_READ_BUF_SIZE;

		
		YY_INPUT( (&yy_current_buffer->yy_ch_buf[number_to_move]),
			yy_n_chars, num_to_read );

		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	if ( yy_n_chars == 0 )
		{
		if ( number_to_move == YY_MORE_ADJ )
			{
			ret_val = EOB_ACT_END_OF_FILE;
			yyrestart( yyin );
			}

		else
			{
			ret_val = EOB_ACT_LAST_MATCH;
			yy_current_buffer->yy_buffer_status =
				YY_BUFFER_EOF_PENDING;
			}
		}

	else
		ret_val = EOB_ACT_CONTINUE_SCAN;

	yy_n_chars += number_to_move;
	yy_current_buffer->yy_ch_buf[yy_n_chars] = YY_END_OF_BUFFER_CHAR;
	yy_current_buffer->yy_ch_buf[yy_n_chars + 1] = YY_END_OF_BUFFER_CHAR;

	yytext_ptr = &yy_current_buffer->yy_ch_buf[0];

	return ret_val;
	}




yy_state_type yyFlexLexer::yy_get_previous_state()
	{
	register yy_state_type yy_current_state;
	register char *yy_cp;

	yy_current_state = yy_start;

	for ( yy_cp = yytext_ptr + YY_MORE_ADJ; yy_cp < yy_c_buf_p; ++yy_cp )
		{
		register YY_CHAR yy_c = (*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : 1);
		if ( yy_accept[yy_current_state] )
			{
			yy_last_accepting_state = yy_current_state;
			yy_last_accepting_cpos = yy_cp;
			}
		while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
			{
			yy_current_state = (int) yy_def[yy_current_state];
			if ( yy_current_state >= 161 )
				yy_c = yy_meta[(unsigned int) yy_c];
			}
		yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
		}

	return yy_current_state;
	}




yy_state_type yyFlexLexer::yy_try_NUL_trans( yy_state_type yy_current_state )
	{
	register int yy_is_jam;
	register char *yy_cp = yy_c_buf_p;

	register YY_CHAR yy_c = 1;
	if ( yy_accept[yy_current_state] )
		{
		yy_last_accepting_state = yy_current_state;
		yy_last_accepting_cpos = yy_cp;
		}
	while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )
		{
		yy_current_state = (int) yy_def[yy_current_state];
		if ( yy_current_state >= 161 )
			yy_c = yy_meta[(unsigned int) yy_c];
		}
	yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];
	yy_is_jam = (yy_current_state == 160);

	return yy_is_jam ? 0 : yy_current_state;
	}


void yyFlexLexer::yyunput( int c, register char* yy_bp )
	{
	register char *yy_cp = yy_c_buf_p;

	
	*yy_cp = yy_hold_char;

	if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
		{ 
		
		register int number_to_move = yy_n_chars + 2;
		register char *dest = &yy_current_buffer->yy_ch_buf[
					yy_current_buffer->yy_buf_size + 2];
		register char *source =
				&yy_current_buffer->yy_ch_buf[number_to_move];

		while ( source > yy_current_buffer->yy_ch_buf )
			*--dest = *--source;

		yy_cp += (int) (dest - source);
		yy_bp += (int) (dest - source);
		yy_current_buffer->yy_n_chars =
			yy_n_chars = yy_current_buffer->yy_buf_size;

		if ( yy_cp < yy_current_buffer->yy_ch_buf + 2 )
			YY_FATAL_ERROR( "flex scanner push-back overflow" );
		}

	*--yy_cp = (char) c;


	yytext_ptr = yy_bp;
	yy_hold_char = *yy_cp;
	yy_c_buf_p = yy_cp;
	}


int yyFlexLexer::yyinput()
	{
	int c;

	*yy_c_buf_p = yy_hold_char;

	if ( *yy_c_buf_p == YY_END_OF_BUFFER_CHAR )
		{
		
		if ( yy_c_buf_p < &yy_current_buffer->yy_ch_buf[yy_n_chars] )
			
			*yy_c_buf_p = '\0';

		else
			{ 
			auto offset = yy_c_buf_p - yytext_ptr;
			++yy_c_buf_p;

			switch ( yy_get_next_buffer() )
				{
				case EOB_ACT_LAST_MATCH:
					

					
					yyrestart( yyin );

					

				case EOB_ACT_END_OF_FILE:
					{
					if ( yywrap() )
						return EOF;

					if ( ! yy_did_buffer_switch_on_eof )
						YY_NEW_FILE;
#ifdef __cplusplus
					return yyinput();
#else
					return input();
#endif
					}

				case EOB_ACT_CONTINUE_SCAN:
					yy_c_buf_p = yytext_ptr + offset;
					break;
				}
			}
		}

	c = *(unsigned char *) yy_c_buf_p;	
	*yy_c_buf_p = '\0';	
	yy_hold_char = *++yy_c_buf_p;


	return c;
	}


void yyFlexLexer::yyrestart( istream* input_file )
	{
	if ( ! yy_current_buffer )
		yy_current_buffer = yy_create_buffer( yyin, YY_BUF_SIZE );

	yy_init_buffer( yy_current_buffer, input_file );
	yy_load_buffer_state();
	}


void yyFlexLexer::yy_switch_to_buffer( YY_BUFFER_STATE new_buffer )
	{
	if ( yy_current_buffer == new_buffer )
		return;

	if ( yy_current_buffer )
		{
		
		*yy_c_buf_p = yy_hold_char;
		yy_current_buffer->yy_buf_pos = yy_c_buf_p;
		yy_current_buffer->yy_n_chars = yy_n_chars;
		}

	yy_current_buffer = new_buffer;
	yy_load_buffer_state();

	
	yy_did_buffer_switch_on_eof = 1;
	}


void yyFlexLexer::yy_load_buffer_state()
	{
	yy_n_chars = yy_current_buffer->yy_n_chars;
	yytext_ptr = yy_c_buf_p = yy_current_buffer->yy_buf_pos;
	yyin = yy_current_buffer->yy_input_file;
	yy_hold_char = *yy_c_buf_p;
	}


YY_BUFFER_STATE yyFlexLexer::yy_create_buffer( istream* file, int size )
	{
	YY_BUFFER_STATE b;

	b = (YY_BUFFER_STATE) yy_flex_alloc( sizeof( struct yy_buffer_state ) );
	if ( ! b )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_buf_size = size;

	
	b->yy_ch_buf = (char *) yy_flex_alloc( b->yy_buf_size + 2 );
	if ( ! b->yy_ch_buf )
		YY_FATAL_ERROR( "out of dynamic memory in yy_create_buffer()" );

	b->yy_is_our_buffer = 1;

	yy_init_buffer( b, file );

	return b;
	}


void yyFlexLexer::yy_delete_buffer( YY_BUFFER_STATE b )
	{
	if ( ! b )
		return;

	if ( b == yy_current_buffer )
		yy_current_buffer = (YY_BUFFER_STATE) 0;

	if ( b->yy_is_our_buffer )
		yy_flex_free( (void *) b->yy_ch_buf );

	yy_flex_free( (void *) b );
	}


extern "C" int isatty YY_PROTO(( int ));
void yyFlexLexer::yy_init_buffer( YY_BUFFER_STATE b, istream* file )

	{
	yy_flush_buffer( b );

	b->yy_input_file = file;
	b->yy_fill_buffer = 1;

	b->yy_is_interactive = 0;
	}


void yyFlexLexer::yy_flush_buffer( YY_BUFFER_STATE b )
	{
	if ( ! b )
		return;

	b->yy_n_chars = 0;

	
	b->yy_ch_buf[0] = YY_END_OF_BUFFER_CHAR;
	b->yy_ch_buf[1] = YY_END_OF_BUFFER_CHAR;

	b->yy_buf_pos = &b->yy_ch_buf[0];

	b->yy_at_bol = 1;
	b->yy_buffer_status = YY_BUFFER_NEW;

	if ( b == yy_current_buffer )
		yy_load_buffer_state();
	}


#ifndef YY_NO_SCAN_BUFFER
#endif


#ifndef YY_NO_SCAN_STRING
#endif


#ifndef YY_NO_SCAN_BYTES
#endif


#ifndef YY_NO_PUSH_STATE
void yyFlexLexer::yy_push_state( int new_state )
	{
	if ( yy_start_stack_ptr >= yy_start_stack_depth )
		{
		yy_size_t new_size;

		yy_start_stack_depth += YY_START_STACK_INCR;
		new_size = yy_start_stack_depth * sizeof( int );

		if ( ! yy_start_stack )
			yy_start_stack = (int *) yy_flex_alloc( new_size );

		else
			yy_start_stack = (int *) yy_flex_realloc(
					(void *) yy_start_stack, new_size );

		if ( ! yy_start_stack )
			YY_FATAL_ERROR(
			"out of memory expanding start-condition stack" );
		}

	yy_start_stack[yy_start_stack_ptr++] = YY_START;

	BEGIN(new_state);
	}
#endif


#ifndef YY_NO_POP_STATE
void yyFlexLexer::yy_pop_state()
	{
	if ( --yy_start_stack_ptr < 0 )
		YY_FATAL_ERROR( "start-condition stack underflow" );

	BEGIN(yy_start_stack[yy_start_stack_ptr]);
	}
#endif


#ifndef YY_NO_TOP_STATE
int yyFlexLexer::yy_top_state()
	{
	return yy_start_stack[yy_start_stack_ptr - 1];
	}
#endif

#ifndef YY_EXIT_FAILURE
#define YY_EXIT_FAILURE 2
#endif


void yyFlexLexer::LexerError( yyconst char msg[] )
	{
	cerr << msg << '\n';
	exit( YY_EXIT_FAILURE );
	}




#undef yyless
#define yyless(n) \
	do \
		{ \
		 \
		yytext[yyleng] = yy_hold_char; \
		yy_c_buf_p = yytext + n; \
		yy_hold_char = *yy_c_buf_p; \
		*yy_c_buf_p = '\0'; \
		yyleng = n; \
		} \
	while ( 0 )




#ifndef yytext_ptr
#ifdef YY_USE_PROTOS
static void yy_flex_strncpy( char *s1, yyconst char *s2, int n )
#else
static void yy_flex_strncpy( s1, s2, n )
char *s1;
yyconst char *s2;
int n;
#endif
	{
	register int i;
	for ( i = 0; i < n; ++i )
		s1[i] = s2[i];
	}
#endif

#ifdef YY_NEED_STRLEN
#ifdef YY_USE_PROTOS
static int yy_flex_strlen( yyconst char *s )
#else
static int yy_flex_strlen( s )
yyconst char *s;
#endif
	{
	register int n;
	for ( n = 0; s[n]; ++n )
		;

	return n;
	}
#endif


#ifdef YY_USE_PROTOS
static void *yy_flex_alloc( yy_size_t size )
#else
static void *yy_flex_alloc( size )
yy_size_t size;
#endif
	{
	return (void *) malloc( size );
	}

#ifdef YY_USE_PROTOS
static void *yy_flex_realloc( void *ptr, yy_size_t size )
#else
static void *yy_flex_realloc( ptr, size )
void *ptr;
yy_size_t size;
#endif
	{
	
	return (void *) realloc( (char *) ptr, size );
	}

#ifdef YY_USE_PROTOS
static void yy_flex_free( void *ptr )
#else
static void yy_flex_free( ptr )
void *ptr;
#endif
	{
	free( ptr );
	}

#if YY_MAIN
int main()
	{
	yylex();
	return 0;
	}
#endif
#line 95 "parser/lexer.lex"





//[\^]		return Parser::token::XOR;



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.