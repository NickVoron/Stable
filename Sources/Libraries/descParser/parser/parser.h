








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
	char strVal[512];
	char name[512];

	Expressions::Expression* expr;
	Expressions::Reference* proxy;
	Expressions::Reference::PathElement* proxyPath;

	Expressions::ConstExprList* guidList;

	PropertyAssignmentList* propertyAssignmentList;
	PropertyAssignment* propertyAssignment;

	ComponentParam* componentParam;
	ComponentParams* componentParams;

	Component* component;
	Expressions::Lambda* lambda;
	Expressions::Callable* callable;




#line 182 "parser/parser.h"
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
     PROPERTY = 270,
     LAMBDA = 271,
     ARRAY = 272,
     NOINSTANCE = 273,
     true_key = 274,
     false_key = 275,
     LESS = 276,
     GREATER = 277,
     EQUAL = 278,
     NOT_EQUAL = 279,
     LESS_EQUAL = 280,
     MORE_EQUAL = 281,
     FORWARD = 282,
     BACKWARD = 283,
     NOT = 284,
     AND = 285,
     OR = 286,
     XOR = 287,
     PLUS = 288,
     MINUS = 289,
     DIV = 290,
     MOD = 291,
     MUL = 292,
     QUERY = 293,
     COLON = 294,
     ITERATOR = 295,
     EXTERNAL = 296,
     PRIVATE = 297,
     RETURN = 298,
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


#line 413 "parser/parser.h"



#endif 
