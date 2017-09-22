// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H




#line 14 "parser/parser.yacc"


#include <string>
#include "../classDescEtc/grammarComposition.h"
#include "../classDescEtc/classDesc.h"
#include "../classDescEtc/component.h"

#ifdef yylex
#undef yylex
#endif

#ifdef THIS
#undef THIS
#endif


namespace ObjectParser
{
	class Lexer;
};







#line 72 "parser/parser.h"


#include <string>
#include <iostream>
#include "stack.hh"



#line 1 "[Bison:b4_percent_define_default]"

namespace ObjectParser {


#line 86 "parser/parser.h"
  class position;
  class location;


#line 1 "[Bison:b4_percent_define_default]"

} 


#line 96 "parser/parser.h"

#include "location.hh"


#ifndef YYDEBUG
# define YYDEBUG 1
#endif


#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif


#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif



#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
do {							\
  if (N)						\
    {							\
      (Current).begin = (Rhs)[1].begin;			\
      (Current).end   = (Rhs)[N].end;			\
    }							\
  else							\
    {							\
      (Current).begin = (Current).end = (Rhs)[0].end;	\
    }							\
} while (false)
#endif



#line 1 "[Bison:b4_percent_define_default]"

namespace ObjectParser {


#line 144 "parser/parser.h"

  
  class Parser
  {
  public:
    
#ifndef YYSTYPE
    union semantic_type
    {


#line 40 "parser/parser.yacc"

	int intVal;
	float fltVal;
	char strVal[128];
	char name[128];

	Expressions::Expression* expr;
	Expressions::Reference* proxy;
	Expressions::Reference::PathElement* proxyPath;

	Expressions::ConstExprList* guidList;

	PropertyAssignmentList* propertyAssignmentList;
	PropertyAssignment* propertyAssignment;

	ComponentParam* componentParam;
	ComponentParams* componentParams;

	Component* component;




#line 180 "parser/parser.h"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    
    typedef location location_type;
    
    struct token
    {
      
   enum yytokentype {
     literal = 258,
     str = 259,
     text = 260,
     flt = 261,
     digit = 262,
     CLASS = 263,
     INTERFACE = 264,
     COMPONENT = 265,
     INCLUDE = 266,
     COLLECTION = 267,
     instance_key = 268,
     THISS = 269,
     VAR = 270,
     PROPERTY = 271,
     LAMBDA = 272,
     ARRAY = 273,
     NOINSTANCE = 274,
     true_key = 275,
     false_key = 276,
     LESS = 277,
     GREATER = 278,
     EQUAL = 279,
     NOT_EQUAL = 280,
     LESS_EQUAL = 281,
     MORE_EQUAL = 282,
     NOT = 283,
     AND = 284,
     OR = 285,
     XOR = 286,
     PLUS = 287,
     MINUS = 288,
     DIV = 289,
     MOD = 290,
     MUL = 291,
     QUERY = 292,
     COLON = 293,
     ITERATOR = 294,
     EXTERNAL = 295,
     PRIVATE = 296,
     AGGREGATE = 297,
     MIX = 298,
     GREATER_EQUAL = 299,
     COMPARISON = 300
   };

    };
    
    typedef token::yytokentype token_type;

    
    Parser (ObjectParser::Lexer& yylex_yyarg);
    virtual ~Parser ();

    
    
    virtual int parse ();

#if YYDEBUG
    
    std::ostream& debug_stream () const;
    
    void set_debug_stream (std::ostream &);

    
    typedef int debug_level_type;
    
    debug_level_type debug_level () const;
    
    void set_debug_level (debug_level_type l);
#endif

  private:
    
    
    
    virtual void error (const location_type& loc, const std::string& msg);

    
    
    
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    
    
    
    
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    
    
    
    
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    
    typedef int state_type;
    
    typedef stack<state_type>    state_stack_type;
    
    typedef stack<semantic_type> semantic_stack_type;
    
    typedef stack<location_type> location_stack_type;

    
    state_stack_type yystate_stack_;
    
    semantic_stack_type yysemantic_stack_;
    
    location_stack_type yylocation_stack_;

    
    typedef unsigned char token_number_type;
    
    
    static const short int yypact_[];
    static const short int yypact_ninf_;

    
    
    
    static const unsigned char yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    
    
    
    
    
    static const short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    
    static const unsigned char yystos_[];

    
    static const unsigned char yyr1_[];
    
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    
    static const char* const yytname_[];
#endif

#if YYERROR_VERBOSE
    
    virtual std::string yytnamerr_ (const char *n);
#endif

#if YYDEBUG
    
    typedef signed char rhs_number_type;
    
    static const rhs_number_type yyrhs_[];
    
    static const unsigned short int yyprhs_[];
    
    static const unsigned short int yyrline_[];
    
    static const unsigned short int yytoken_number_[];
    
    virtual void yy_reduce_print_ (int r);
    
    virtual void yystack_print_ ();

    
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    
    token_number_type yytranslate_ (int t);

    
    
    
    
    
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    
    inline void yypop_ (unsigned int n = 1);

    
    static const int yyeof_;
    
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    
    ObjectParser::Lexer& yylex;
  };


#line 1 "[Bison:b4_percent_define_default]"

} 


#line 411 "parser/parser.h"



#endif 




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