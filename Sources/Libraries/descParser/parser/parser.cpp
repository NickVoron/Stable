// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//









#define yylex   ObjectParserlex





#line 43 "parser/parser.cpp"


#include "parser.h"




#line 61 "parser/parser.yacc"

	#include "lexer.h"
	#include <iostream>
	#include "../compiler/compiler.h"



#line 59 "parser/parser.cpp"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> 
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


#define YYUSE(e) ((void) (e))


#if YYDEBUG


# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else 

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif 

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)



#line 1 "[Bison:b4_percent_define_default]"

namespace ObjectParser {


#line 128 "parser/parser.cpp"
#if YYERROR_VERBOSE

  
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  
  Parser::Parser (ObjectParser::Lexer& yylex_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      yylex (yylex_yyarg)
  {
  }

  Parser::~Parser ()
  {
  }

#if YYDEBUG
  

  inline void
  Parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  Parser::parse ()
  {
    
    int yychar = yyempty_;
    int yytoken = 0;

    
    int yyn;
    int yylen = 0;
    int yystate = 0;

    
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    
    semantic_type yylval;
    
    location_type yylloc;
    
    location_type yyerror_range[2];

    
    semantic_type yyval;
    
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    
    

#line 8 "parser/parser.yacc"
{
	yylloc.initialize(&yylex.fileName);
}


#line 311 "parser/parser.cpp"

    
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    
  yybackup:

    
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  
  yyreduce:
    yylen = yyr2_[yyn];
    
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 4:


#line 141 "parser/parser.yacc"
    {	yylex.compiler.loadClassDescInfo( boost::filesystem::path(yylex.fileName).parent_path() / (yysemantic_stack_[(2) - (2)].strVal) );		}
    break;

  case 5:


#line 142 "parser/parser.yacc"
    {	yylex.gc.endClass();	}
    break;

  case 8:


#line 149 "parser/parser.yacc"
    { yylex.gc.endCollection((yysemantic_stack_[(8) - (2)].name), (yysemantic_stack_[(8) - (4)].name));	}
    break;

  case 9:


#line 150 "parser/parser.yacc"
    { yylex.gc.endCollection((yysemantic_stack_[(7) - (2)].name), (yysemantic_stack_[(7) - (4)].name));	}
    break;

  case 12:


#line 159 "parser/parser.yacc"
    {	yylex.gc.addCollectionElement((yysemantic_stack_[(2) - (1)].name), (yysemantic_stack_[(2) - (2)].guidList)); }
    break;

  case 13:


#line 163 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), false);	}
    break;

  case 14:


#line 164 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), false);	}
    break;

  case 15:


#line 165 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), false);	}
    break;

  case 16:


#line 166 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), false);	}
    break;

  case 17:


#line 167 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), true);	}
    break;

  case 18:


#line 168 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), true);	}
    break;

  case 19:


#line 169 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), true);	}
    break;

  case 20:


#line 170 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), true);	}
    break;

  case 22:


#line 175 "parser/parser.yacc"
    {			}
    break;

  case 23:


#line 179 "parser/parser.yacc"
    {		}
    break;

  case 24:


#line 180 "parser/parser.yacc"
    {		}
    break;

  case 25:


#line 184 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(2) - (2)].name), nullptr);		}
    break;

  case 26:


#line 185 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(4) - (2)].name), nullptr);		}
    break;

  case 27:


#line 186 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(5) - (2)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(5) - (4)].propertyAssignmentList)));	}
    break;

  case 28:


#line 190 "parser/parser.yacc"
    { yylex.gc.addAggregateInheritance((yysemantic_stack_[(2) - (2)].name));		}
    break;

  case 36:


#line 207 "parser/parser.yacc"
    {	}
    break;

  case 37:


#line 208 "parser/parser.yacc"
    {	yylex.gc.setInheritanceAssigment((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList)));	}
    break;

  case 38:


#line 212 "parser/parser.yacc"
    { yylex.gc.newInstance((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (2)].name), nullptr); }
    break;

  case 39:


#line 213 "parser/parser.yacc"
    { yylex.gc.newInstance((yysemantic_stack_[(5) - (1)].name), (yysemantic_stack_[(5) - (2)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(5) - (4)].propertyAssignmentList))); }
    break;

  case 40:


#line 214 "parser/parser.yacc"
    { yylex.gc.newInstanceArray((yysemantic_stack_[(7) - (1)].name), (yysemantic_stack_[(7) - (2)].name), (yysemantic_stack_[(7) - (4)].expr), nullptr); }
    break;

  case 41:


#line 215 "parser/parser.yacc"
    { yylex.gc.newInstanceArray((yysemantic_stack_[(8) - (1)].name), (yysemantic_stack_[(8) - (2)].name), (yysemantic_stack_[(8) - (4)].expr), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(8) - (7)].propertyAssignmentList))); }
    break;

  case 42:


#line 219 "parser/parser.yacc"
    {	yylex.gc.addvar((yysemantic_stack_[(4) - (2)].name), (yysemantic_stack_[(4) - (4)].expr));	}
    break;

  case 43:


#line 223 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(2) - (2)].name));		}
    break;

  case 44:


#line 224 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(4) - (2)].name),(yysemantic_stack_[(4) - (4)].expr));	}
    break;

  case 45:


#line 225 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(5) - (3)].name),(yysemantic_stack_[(5) - (5)].expr), true); }
    break;

  case 46:


#line 229 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].expr));				}
    break;

  case 47:


#line 230 "parser/parser.yacc"
    { (yyval.propertyAssignment) = 0; yylex.gc.bindRuntimeProperty((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (4)].proxy), In);			}
    break;

  case 48:


#line 231 "parser/parser.yacc"
    { (yyval.propertyAssignment) = 0; yylex.gc.bindRuntimeProperty((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (4)].proxy), Out);		}
    break;

  case 49:


#line 236 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(1) - (1)].component), nullptr); }
    break;

  case 50:


#line 237 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(3) - (1)].component), nullptr); }
    break;

  case 51:


#line 238 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(4) - (1)].component), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 52:


#line 242 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(3) - (2)].name), (yysemantic_stack_[(3) - (3)].name));																		}
    break;

  case 53:


#line 243 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(6) - (2)].name), (yysemantic_stack_[(6) - (3)].name), std::unique_ptr<ComponentParams>((yysemantic_stack_[(6) - (5)].componentParams)));								}
    break;

  case 54:


#line 244 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(2) - (2)].name), str::stringize((yysemantic_stack_[(2) - (2)].name), "_", ComponentNameSource::strGuid()));											}
    break;

  case 55:


#line 245 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(5) - (2)].name), str::stringize((yysemantic_stack_[(5) - (2)].name), "_", ComponentNameSource::strGuid()), std::unique_ptr<ComponentParams>((yysemantic_stack_[(5) - (4)].componentParams)));	}
    break;

  case 57:


#line 250 "parser/parser.yacc"
    {		}
    break;

  case 58:


#line 251 "parser/parser.yacc"
    { yylex.gc.endComponentRuntimeParams(std::unique_ptr<ComponentParams>((yysemantic_stack_[(4) - (3)].componentParams)));	}
    break;

  case 59:


#line 256 "parser/parser.yacc"
    {
		ComponentParams* params = new ComponentParams();
		params->emplace_back((yysemantic_stack_[(1) - (1)].componentParam));
		(yyval.componentParams)=params;
	}
    break;

  case 60:


#line 261 "parser/parser.yacc"
    { (yyval.componentParams)->emplace_back((yysemantic_stack_[(3) - (3)].componentParam)); }
    break;

  case 61:


#line 266 "parser/parser.yacc"
    {	
		ComponentParam* param = new ComponentParam((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].name));
		(yyval.componentParam) = param;
	}
    break;

  case 62:


#line 273 "parser/parser.yacc"
    { (yyval.guidList) = new Expressions::ConstExprList((yysemantic_stack_[(1) - (1)].expr)); }
    break;

  case 63:


#line 274 "parser/parser.yacc"
    { (yysemantic_stack_[(3) - (1)].guidList)->push_back((yysemantic_stack_[(3) - (3)].expr)); (yyval.guidList)=(yysemantic_stack_[(3) - (1)].guidList);		}
    break;

  case 64:


#line 278 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 65:


#line 279 "parser/parser.yacc"
    {	ENFORCE(true);	}
    break;

  case 66:


#line 280 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].proxy);		}
    break;

  case 67:


#line 281 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 68:


#line 282 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 69:


#line 283 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(3) - (2)].expr);		}
    break;

  case 70:


#line 284 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 71:


#line 285 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 72:


#line 286 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 73:


#line 287 "parser/parser.yacc"
    {}
    break;

  case 74:


#line 291 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(3) - (1)].name), nullptr); }
    break;

  case 75:


#line 292 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 76:


#line 296 "parser/parser.yacc"
    { (yyval.propertyAssignmentList) = yylex.gc.newPropertyAssignmentsList((yysemantic_stack_[(1) - (1)].propertyAssignment));	   }
    break;

  case 77:


#line 297 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(2) - (1)].propertyAssignmentList), (yysemantic_stack_[(2) - (2)].propertyAssignment)); }
    break;

  case 78:


#line 298 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(3) - (1)].propertyAssignmentList), (yysemantic_stack_[(3) - (3)].propertyAssignment)); }
    break;

  case 79:


#line 303 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.addExternalExtension((yysemantic_stack_[(2) - (2)].name), std::string() ); }
    break;

  case 80:


#line 307 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newTernaryOperator((yysemantic_stack_[(5) - (1)].expr), (yysemantic_stack_[(5) - (3)].expr), (yysemantic_stack_[(5) - (5)].expr)); }
    break;

  case 81:


#line 311 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("less",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 82:


#line 312 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("greater",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 83:


#line 313 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("lequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 84:


#line 314 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("gequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 85:


#line 315 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("equal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 86:


#line 316 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("notequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 87:


#line 317 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("and",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 88:


#line 318 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("or",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 89:


#line 319 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("not",		Expressions::ConstExprList((yysemantic_stack_[(2) - (2)].expr)));		}
    break;

  case 90:


#line 320 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("add", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 91:


#line 321 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("sub", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 92:


#line 322 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mul", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 93:


#line 323 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("div", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 94:


#line 324 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mod", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 95:


#line 328 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newLambda((yysemantic_stack_[(4) - (3)].name), *(yysemantic_stack_[(4) - (4)].guidList));	}
    break;

  case 96:


#line 332 "parser/parser.yacc"
    {	(yyval.guidList) = (yysemantic_stack_[(3) - (2)].guidList);					}
    break;

  case 97:


#line 333 "parser/parser.yacc"
    {	(yyval.guidList) = new Expressions::ConstExprList();	}
    break;

  case 98:


#line 338 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(3) - (2)].propertyAssignmentList))); }
    break;

  case 99:


#line 343 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(1) - (1)].proxy);}
    break;

  case 100:


#line 344 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(2) - (1)].proxy); (yysemantic_stack_[(2) - (1)].proxy)->addPathElement((yysemantic_stack_[(2) - (2)].proxyPath));	}
    break;

  case 101:


#line 345 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 102:


#line 346 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 103:


#line 350 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy();					}
    break;

  case 104:


#line 351 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy("iterator");		}
    break;

  case 105:


#line 352 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy((yysemantic_stack_[(1) - (1)].name));				}
    break;

  case 106:


#line 356 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyArrayPath((yysemantic_stack_[(3) - (2)].intVal)); }
    break;

  case 107:


#line 360 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyComponentPath((yysemantic_stack_[(3) - (2)].name)); }
    break;

  case 108:


#line 364 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyPropertyPath((yysemantic_stack_[(1) - (1)].name)); }
    break;

  case 109:


#line 369 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunctionOrStruct((yysemantic_stack_[(2) - (1)].name), std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(2) - (2)].guidList)));		}
    break;

  case 110:


#line 370 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].intVal));						}
    break;

  case 111:


#line 371 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(std::string((yysemantic_stack_[(1) - (1)].strVal)));			}
    break;

  case 112:


#line 372 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].fltVal));						}
    break;

  case 113:


#line 373 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(true);						}
    break;

  case 114:


#line 374 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(false);					}
    break;

  case 115:


#line 375 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(3) - (2)].guidList)));	 					}
    break;




#line 1130 "parser/parser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  
  yyerrlab:
    
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	

	if (yychar <= yyeof_)
	  {
	  
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    
    goto yyerrlab1;


  
  yyerrorlab:

    
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  
  yyerrlab1:
    yyerrstatus_ = 3;	

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    
    
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  
  std::string
  Parser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	
	int yyxbegin = yyn < 0 ? -yyn : 0;

	
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	
	
	
	
	
	
	
	
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  
  const signed char Parser::yypact_ninf_ = -128;
  const short int
  Parser::yypact_[] =
  {
       308,  -128,    15,    29,    35,    45,   230,  -128,  -128,  -128,
     -10,    85,  -128,    24,  -128,  -128,   155,   164,   159,   164,
     109,   133,    63,   139,   145,   156,  -128,   175,  -128,  -128,
    -128,  -128,  -128,  -128,   128,   181,   198,   214,    59,  -128,
    -128,  -128,  -128,   177,    86,   172,   -41,    25,    -1,   147,
     165,   220,  -128,  -128,    70,     1,  -128,   173,   179,   164,
    -128,   195,   184,    94,   125,   200,  -128,  -128,    17,   171,
    -128,   -20,  -128,   125,   125,   187,  -128,    18,   185,   242,
       5,  -128,   199,  -128,  -128,   219,    95,  -128,    31,  -128,
      -7,  -128,  -128,  -128,  -128,  -128,  -128,   125,  -128,   244,
     125,    23,   246,   229,  -128,  -128,  -128,  -128,  -128,  -128,
     -40,  -128,  -128,   176,   125,   203,  -128,   244,  -128,   264,
     242,  -128,   301,   301,   125,  -128,   242,    87,  -128,    12,
    -128,  -128,   217,  -128,   108,  -128,  -128,   110,   102,  -128,
     190,    69,   253,   267,    49,   301,  -128,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   223,   265,   121,  -128,     8,   301,     8,  -128,  -128,
    -128,   301,   115,  -128,  -128,  -128,  -128,  -128,  -128,    76,
    -128,   135,  -128,  -128,   217,   125,  -128,   324,   324,   324,
     324,   324,   317,   317,   258,   258,   190,   190,   190,   278,
     324,   112,   236,  -128,   271,  -128,  -128,  -128,   -40,   -40,
    -128,  -128,  -128,  -128,   301,   125,  -128,    98,  -128,   248,
     301,  -128,  -128
  };

  
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     7,     0,     0,     0,     0,     0,     2,     6,     5,
       0,     0,     4,     0,     1,     3,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,    13,     0,    29,    35,
      34,    32,    33,    31,    49,     0,     0,     0,     0,    22,
      23,    24,    17,     0,     0,     0,     0,     0,     0,     0,
      43,     0,    14,    30,     0,    54,    28,    25,     0,     0,
      18,     0,     0,     0,     0,     0,    36,    76,     0,     0,
      57,     0,    59,     0,     0,     0,    50,     0,    52,     0,
       0,    15,     0,    21,    19,     0,     0,    38,     0,    73,
     105,   111,   112,   110,   103,   113,   114,     0,   104,     0,
       0,     0,     0,     0,    70,    72,    67,    68,    65,    71,
      66,    99,    64,     0,     0,     0,    37,     0,    77,     0,
       0,    58,    42,    44,     0,    51,     0,     0,    26,     0,
      16,    20,     0,     9,     0,    10,    39,     0,     0,   109,
      89,     0,     0,     0,     0,    62,    79,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,    46,     0,    78,    61,
      60,    45,     0,    55,    27,    12,     8,    11,    74,     0,
      97,     0,    98,    69,     0,     0,   115,    81,    82,    85,
      86,    83,    87,    88,    90,    91,    93,    94,    92,     0,
      84,     0,     0,   108,     0,   101,   102,   105,    47,    48,
      53,    75,    96,    95,    63,     0,    40,     0,   106,     0,
      80,    41,   107
  };

  
  const short int
  Parser::yypgoto_[] =
  {
      -128,  -128,   290,  -128,  -128,   210,  -128,   279,   240,  -128,
    -128,   -12,   -26,  -128,  -128,  -128,  -128,   -65,  -128,  -128,
    -128,   -24,   201,   167,   -64,  -128,   -38,  -128,  -128,  -128,
    -128,  -127,  -128,    32,  -128,  -128,  -128,  -128,  -128
  };

  
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     6,     7,     8,   134,   135,     9,    38,    39,    40,
      41,    27,    28,    29,    30,    31,    32,    67,    33,    34,
      35,    71,    72,   144,   145,   104,    68,   105,   106,   107,
     108,   139,   109,   110,   111,   164,   205,   206,   112
  };

  
  const signed char Parser::yytable_ninf_ = -1;
  const unsigned char
  Parser::yytable_[] =
  {
       103,    53,    69,   118,    78,   175,    43,    63,    65,   122,
     123,   207,   118,    64,   162,    65,    77,    53,    10,   163,
      65,    65,    94,   118,    89,    88,    90,    91,    65,    92,
      93,   120,    11,   140,    65,   121,   142,    94,    16,    12,
      17,   137,   129,    95,    96,   138,    82,    98,    13,    85,
     166,    97,   168,    79,    70,   127,    53,   213,   128,    53,
     171,   141,    98,   117,   118,   174,   116,   125,   117,   117,
      20,    99,    65,    65,    66,   100,   118,   101,   143,    65,
     136,   102,   117,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    65,   132,   179,
     185,    65,   172,    89,   186,    90,    91,    58,    92,    93,
      59,   132,    45,    65,   118,    65,    94,    48,   182,    76,
     117,   214,    95,    96,   203,   211,    89,   117,    90,    91,
      97,    92,    93,    18,    61,    19,    46,    59,   120,    94,
     173,    98,    49,    87,   133,    95,    96,   221,    50,   117,
      99,   220,   118,    97,   100,   180,   101,   176,    21,   178,
     102,   216,    21,   217,    98,    22,   120,   204,   210,    22,
      23,    24,    51,    99,    23,    24,    54,   100,    21,   101,
      21,    47,    21,   102,    55,    22,   185,    22,   212,    22,
      23,    24,    23,    24,    23,    24,    25,   208,    21,   209,
      25,    56,    21,    73,    26,    22,    36,    37,    42,    22,
      23,    24,   147,   148,    23,    24,    25,    57,    25,    62,
      25,    74,    21,    75,    52,    80,    60,   119,    81,    22,
      14,     1,    86,   165,    23,    24,    25,   126,     2,     3,
      25,     4,     5,   124,    84,    69,   113,    65,   130,   146,
     167,   147,   148,   149,   150,   151,   114,   115,   152,   153,
      25,   154,   155,   156,   157,   158,   159,   169,   131,   138,
     184,   201,   202,   160,   219,   147,   148,   149,   150,   151,
     147,   148,   152,   153,   161,   154,   155,   156,   157,   158,
     159,   218,   156,   157,   158,   222,    15,   160,    44,    83,
     147,   148,   149,   150,   151,   181,   183,   152,   153,     1,
     154,   155,   156,   157,   158,   159,     2,     3,     0,     4,
       5,   170,   160,   147,   148,   149,   150,   151,   215,     0,
     152,   153,     0,   154,   155,   156,   157,   158,   159,   147,
     148,   149,   150,   151,   177,   160,   147,   148,     0,   154,
     155,   156,   157,   158,     0,     0,   154,   155,   156,   157,
     158,   160
  };

  
  const short int
  Parser::yycheck_[] =
  {
        64,    27,     3,    68,     3,   132,    18,    48,     3,    73,
      74,     3,    77,    54,    54,     3,    54,    43,     3,    59,
       3,     3,    14,    88,     1,    63,     3,     4,     3,     6,
       7,    51,     3,    97,     3,    55,   100,    14,    48,     4,
      50,    48,    80,    20,    21,    52,    58,    39,     3,    61,
     114,    28,   117,    52,    55,    79,    82,   184,    53,    85,
     124,    99,    39,    51,   129,    53,    49,    49,    51,    51,
      46,    48,     3,     3,    49,    52,   141,    54,    55,     3,
      49,    58,    51,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,     3,     3,   137,
      51,     3,   126,     1,    55,     3,     4,    48,     6,     7,
      51,     3,     3,     3,   179,     3,    14,    54,    49,    49,
      51,   185,    20,    21,     3,    49,     1,    51,     3,     4,
      28,     6,     7,    48,    48,    50,     3,    51,    51,    14,
      53,    39,     3,    49,    49,    20,    21,    49,     3,    51,
      48,   215,   217,    28,    52,    53,    54,    49,     3,    49,
      58,    49,     3,   201,    39,    10,    51,    46,    53,    10,
      15,    16,    16,    48,    15,    16,    48,    52,     3,    54,
       3,    48,     3,    58,     3,    10,    51,    10,    53,    10,
      15,    16,    15,    16,    15,    16,    41,   165,     3,   167,
      41,     3,     3,    56,    49,    10,    42,    43,    49,    10,
      15,    16,    22,    23,    15,    16,    41,     3,    41,    47,
      41,    56,     3,     3,    49,    52,    49,    56,    49,    10,
       0,     1,    48,    57,    15,    16,    41,    52,     8,     9,
      41,    11,    12,    56,    49,     3,    46,     3,    49,     3,
      47,    22,    23,    24,    25,    26,    56,    57,    29,    30,
      41,    32,    33,    34,    35,    36,    37,     3,    49,    52,
       3,    48,     7,    44,     3,    22,    23,    24,    25,    26,
      22,    23,    29,    30,    55,    32,    33,    34,    35,    36,
      37,    55,    34,    35,    36,    47,     6,    44,    19,    59,
      22,    23,    24,    25,    26,   138,    53,    29,    30,     1,
      32,    33,    34,    35,    36,    37,     8,     9,    -1,    11,
      12,   120,    44,    22,    23,    24,    25,    26,    50,    -1,
      29,    30,    -1,    32,    33,    34,    35,    36,    37,    22,
      23,    24,    25,    26,   134,    44,    22,    23,    -1,    32,
      33,    34,    35,    36,    -1,    -1,    32,    33,    34,    35,
      36,    44
  };

  
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     8,     9,    11,    12,    61,    62,    63,    66,
       3,     3,     4,     3,     0,    62,    48,    50,    48,    50,
      46,     3,    10,    15,    16,    41,    49,    71,    72,    73,
      74,    75,    76,    78,    79,    80,    42,    43,    67,    68,
      69,    70,    49,    71,    67,     3,     3,    48,    54,     3,
       3,    16,    49,    72,    48,     3,     3,     3,    48,    51,
      49,    48,    47,    48,    54,     3,    49,    77,    86,     3,
      55,    81,    82,    56,    56,     3,    49,    86,     3,    52,
      52,    49,    71,    68,    49,    71,    48,    49,    86,     1,
       3,     4,     6,     7,    14,    20,    21,    28,    39,    48,
      52,    54,    58,    84,    85,    87,    88,    89,    90,    92,
      93,    94,    98,    46,    56,    57,    49,    51,    77,    56,
      51,    55,    84,    84,    56,    49,    52,    81,    53,    86,
      49,    49,     3,    49,    64,    65,    49,    48,    52,    91,
      84,    86,    84,    55,    83,    84,     3,    22,    23,    24,
      25,    26,    29,    30,    32,    33,    34,    35,    36,    37,
      44,    55,    54,    59,    95,    57,    84,    47,    77,     3,
      82,    84,    81,    53,    53,    91,    49,    65,    49,    86,
      53,    83,    49,    53,     3,    51,    55,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    48,     7,     3,    46,    96,    97,     3,    93,    93,
      53,    49,    53,    91,    84,    50,    49,    86,    55,     3,
      84,    49,    47
  };

#if YYDEBUG
  
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    60,    62,   123,   125,
      58,    44,    40,    41,    91,    93,    61,    45,    35,    46
  };
#endif

  
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    60,    61,    61,    62,    62,    62,    62,    63,    63,
      64,    64,    65,    66,    66,    66,    66,    66,    66,    66,
      66,    67,    67,    68,    68,    69,    69,    69,    70,    71,
      71,    72,    72,    72,    72,    72,    73,    73,    74,    74,
      74,    74,    75,    76,    76,    76,    77,    77,    77,    78,
      78,    78,    79,    79,    79,    79,    80,    80,    80,    81,
      81,    82,    83,    83,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    85,    85,    86,    86,    86,    87,
      88,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    91,    91,    92,    93,
      93,    93,    93,    94,    94,    94,    95,    96,    97,    98,
      98,    98,    98,    98,    98,    98
  };

  
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     2,     2,     1,     1,     1,     8,     7,
       1,     2,     2,     4,     5,     6,     7,     4,     5,     6,
       7,     3,     1,     1,     1,     2,     4,     5,     2,     1,
       2,     1,     1,     1,     1,     1,     3,     4,     4,     5,
       7,     8,     4,     2,     4,     5,     3,     4,     4,     1,
       3,     4,     3,     6,     2,     5,     1,     3,     4,     1,
       3,     3,     1,     3,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     3,     4,     1,     2,     3,     2,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     3,     3,     3,     4,     3,     2,     3,     1,
       2,     3,     3,     1,     1,     1,     3,     3,     1,     2,
       1,     1,     1,     1,     1,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  
  const char*
  const Parser::yytname_[] =
  {
    "$end", "error", "$undefined", "literal", "str", "text", "flt", "digit",
  "\"class definition\"", "\"interface def\"", "\"component definition\"",
  "\"include\"", "\"collection definition\"", "instance_key", "\"this\"",
  "VAR", "PROPERTY", "LAMBDA", "ARRAY", "NOINSTANCE", "true_key",
  "false_key", "LESS", "GREATER", "EQUAL", "NOT_EQUAL", "LESS_EQUAL",
  "MORE_EQUAL", "NOT", "AND", "OR", "XOR", "PLUS", "MINUS", "DIV", "MOD",
  "MUL", "QUERY", "COLON", "ITERATOR", "EXTERNAL", "PRIVATE",
  "\"aggreate inheriatnce\"", "\"mix inheritance\"", "GREATER_EQUAL",
  "COMPARISON", "'<'", "'>'", "'{'", "'}'", "':'", "','", "'('", "')'",
  "'['", "']'", "'='", "'-'", "'#'", "'.'", "$accept", "file",
  "file_element", "collection_def", "collection_var_list",
  "collection_element", "class_def", "parent_list", "inheritanceDef",
  "mixInheritance", "aggregateInheritance", "body", "element",
  "aggregateInheritance_init", "instance_def", "var", "property_def",
  "property_assignment", "component", "component_def",
  "component_runtime_params", "component_params", "component_param",
  "exp_list", "exp", "type_def", "instance_body", "external_language",
  "if_operator", "operators", "lambda", "call_operator", "anonymousStruct",
  "proxy", "proxy_head", "array_path_element", "components_path_element",
  "property_path_element", "const", 0
  };
#endif

#if YYDEBUG
  
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        61,     0,    -1,    62,    -1,    61,    62,    -1,    11,     4,
      -1,    66,    -1,    63,    -1,     1,    -1,    12,     3,    46,
       3,    47,    48,    64,    49,    -1,    12,     3,    46,     3,
      47,    48,    49,    -1,    65,    -1,    64,    65,    -1,     3,
      91,    -1,     8,     3,    48,    49,    -1,     8,     3,    48,
      71,    49,    -1,     8,     3,    50,    67,    48,    49,    -1,
       8,     3,    50,    67,    48,    71,    49,    -1,     9,     3,
      48,    49,    -1,     9,     3,    48,    71,    49,    -1,     9,
       3,    50,    67,    48,    49,    -1,     9,     3,    50,    67,
      48,    71,    49,    -1,    67,    51,    68,    -1,    68,    -1,
      69,    -1,    70,    -1,    43,     3,    -1,    43,     3,    52,
      53,    -1,    43,     3,    52,    86,    53,    -1,    42,     3,
      -1,    72,    -1,    71,    72,    -1,    78,    -1,    75,    -1,
      76,    -1,    74,    -1,    73,    -1,     3,    48,    49,    -1,
       3,    48,    86,    49,    -1,     3,     3,    48,    49,    -1,
       3,     3,    48,    86,    49,    -1,     3,     3,    54,    84,
      55,    48,    49,    -1,     3,     3,    54,    84,    55,    48,
      86,    49,    -1,    15,     3,    56,    84,    -1,    16,     3,
      -1,    16,     3,    56,    84,    -1,    41,    16,     3,    56,
      84,    -1,     3,    56,    84,    -1,     3,    46,    57,    93,
      -1,     3,    57,    47,    93,    -1,    79,    -1,    79,    48,
      49,    -1,    79,    48,    86,    49,    -1,    80,     3,     3,
      -1,    80,     3,     3,    52,    81,    53,    -1,    80,     3,
      -1,    80,     3,    52,    81,    53,    -1,    10,    -1,    10,
      54,    55,    -1,    10,    54,    81,    55,    -1,    82,    -1,
      81,    51,    82,    -1,     3,    56,     3,    -1,    84,    -1,
      83,    51,    84,    -1,    98,    -1,    90,    -1,    93,    -1,
      88,    -1,    89,    -1,    52,    84,    53,    -1,    85,    -1,
      92,    -1,    87,    -1,     1,    -1,     3,    48,    49,    -1,
       3,    48,    86,    49,    -1,    77,    -1,    86,    77,    -1,
      86,    51,    77,    -1,    58,     3,    -1,    84,    37,    84,
      50,    84,    -1,    84,    22,    84,    -1,    84,    23,    84,
      -1,    84,    26,    84,    -1,    84,    44,    84,    -1,    84,
      24,    84,    -1,    84,    25,    84,    -1,    84,    29,    84,
      -1,    84,    30,    84,    -1,    28,    84,    -1,    84,    32,
      84,    -1,    84,    33,    84,    -1,    84,    36,    84,    -1,
      84,    34,    84,    -1,    84,    35,    84,    -1,    54,    55,
       3,    91,    -1,    52,    83,    53,    -1,    52,    53,    -1,
      48,    86,    49,    -1,    94,    -1,    93,    95,    -1,    93,
      59,    96,    -1,    93,    59,    97,    -1,    14,    -1,    39,
      -1,     3,    -1,    54,     7,    55,    -1,    46,     3,    47,
      -1,     3,    -1,     3,    91,    -1,     7,    -1,     4,    -1,
       6,    -1,    20,    -1,    21,    -1,    54,    83,    55,    -1
  };

  
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,    11,    13,    15,    17,    26,
      34,    36,    39,    42,    47,    53,    60,    68,    73,    79,
      86,    94,    98,   100,   102,   104,   107,   112,   118,   121,
     123,   126,   128,   130,   132,   134,   136,   140,   145,   150,
     156,   164,   173,   178,   181,   186,   192,   196,   201,   206,
     208,   212,   217,   221,   228,   231,   237,   239,   243,   248,
     250,   254,   258,   260,   264,   266,   268,   270,   272,   274,
     278,   280,   282,   284,   286,   290,   295,   297,   300,   304,
     307,   313,   317,   321,   325,   329,   333,   337,   341,   345,
     348,   352,   356,   360,   364,   368,   373,   377,   380,   384,
     386,   389,   393,   397,   399,   401,   403,   407,   411,   413,
     416,   418,   420,   422,   424,   426
  };

  
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   136,   136,   137,   141,   142,   143,   144,   149,   150,
     154,   155,   159,   163,   164,   165,   166,   167,   168,   169,
     170,   174,   175,   179,   180,   184,   185,   186,   190,   194,
     195,   199,   200,   201,   202,   203,   207,   208,   212,   213,
     214,   215,   219,   223,   224,   225,   229,   230,   231,   236,
     237,   238,   242,   243,   244,   245,   249,   250,   251,   255,
     261,   265,   273,   274,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   291,   292,   296,   297,   298,   303,
     307,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   328,   332,   333,   338,   343,
     344,   345,   346,   350,   351,   352,   356,   360,   364,   369,
     370,   371,   372,   373,   374,   375
  };

  
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif 

  
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    58,     2,     2,     2,     2,
      52,    53,     2,     2,    51,    57,    59,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    50,     2,
      46,    56,    47,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    54,     2,    55,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 361;
  const int Parser::yynnts_ = 39;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 14;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 60;

  const unsigned int Parser::yyuser_token_number_max_ = 300;
  const Parser::token_number_type Parser::yyundef_token_ = 2;



#line 1 "[Bison:b4_percent_define_default]"

} 


#line 1795 "parser/parser.cpp"



#line 380 "parser/parser.yacc"

void ObjectParser::Parser::error(const location_type& loc, const std::string& msg)
{
	LOG_ERROR("parser error in file \"" <<  yylex.fileName << "\" at: "<< loc << msg);
}





// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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