







#define yylex   ObjectParserlex





#line 43 "parser/parser.cpp"


#include "parser.h"




#line 63 "parser/parser.yacc"

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


#line 148 "parser/parser.yacc"
    {	yylex.compiler.loadClassDescInfo( boost::filesystem::path(yylex.fileName).parent_path() / (yysemantic_stack_[(2) - (2)].strVal) );		}
    break;

  case 5:


#line 149 "parser/parser.yacc"
    {	yylex.gc.endClass();	}
    break;

  case 7:


#line 154 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), false);	}
    break;

  case 8:


#line 155 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), false);	}
    break;

  case 9:


#line 156 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), false);	}
    break;

  case 10:


#line 157 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), false);	}
    break;

  case 11:


#line 158 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), true);	}
    break;

  case 12:


#line 159 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), true);	}
    break;

  case 13:


#line 160 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), true);	}
    break;

  case 14:


#line 161 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), true);	}
    break;

  case 16:


#line 166 "parser/parser.yacc"
    {			}
    break;

  case 17:


#line 170 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(1) - (1)].name), nullptr);		}
    break;

  case 18:


#line 171 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(3) - (1)].name), nullptr);		}
    break;

  case 19:


#line 172 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList)));	}
    break;

  case 25:


#line 184 "parser/parser.yacc"
    { yylex.gc.addProperty( yylex.gc.newCallable((yysemantic_stack_[(2) - (1)].name), std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(2) - (2)].guidList))));	}
    break;

  case 26:


#line 185 "parser/parser.yacc"
    { yylex.gc.addProperty( (yysemantic_stack_[(1) - (1)].proxy) );						}
    break;

  case 27:


#line 189 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(3) - (1)].name), nullptr); }
    break;

  case 28:


#line 190 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 29:


#line 191 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (2)].name), nullptr); }
    break;

  case 30:


#line 192 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(5) - (1)].name), (yysemantic_stack_[(5) - (2)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(5) - (4)].propertyAssignmentList))); }
    break;

  case 31:


#line 193 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(6) - (1)].name), (yysemantic_stack_[(6) - (3)].expr), nullptr); }
    break;

  case 32:


#line 194 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(7) - (1)].name), (yysemantic_stack_[(7) - (3)].expr), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(7) - (6)].propertyAssignmentList))); }
    break;

  case 33:


#line 195 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(7) - (1)].name), (yysemantic_stack_[(7) - (2)].name), (yysemantic_stack_[(7) - (4)].expr), nullptr); }
    break;

  case 34:


#line 196 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(8) - (1)].name), (yysemantic_stack_[(8) - (2)].name), (yysemantic_stack_[(8) - (4)].expr), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(8) - (7)].propertyAssignmentList))); }
    break;

  case 35:


#line 200 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(2) - (2)].name));		}
    break;

  case 36:


#line 201 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(4) - (2)].name), (yysemantic_stack_[(4) - (4)].expr));	}
    break;

  case 37:


#line 205 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].expr));			}
    break;

  case 38:


#line 209 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newConnection((yysemantic_stack_[(3) - (1)].proxy), (yysemantic_stack_[(3) - (3)].expr), Connection::Backward); }
    break;

  case 39:


#line 210 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newConnection((yysemantic_stack_[(3) - (1)].proxy), (yysemantic_stack_[(3) - (3)].expr), Connection::Forward); }
    break;

  case 40:


#line 214 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(1) - (1)].component), nullptr); }
    break;

  case 41:


#line 215 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(3) - (1)].component), nullptr); }
    break;

  case 42:


#line 216 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(4) - (1)].component), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 43:


#line 220 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(3) - (2)].name), (yysemantic_stack_[(3) - (3)].name));																		}
    break;

  case 44:


#line 221 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(6) - (2)].name), (yysemantic_stack_[(6) - (3)].name), std::unique_ptr<ComponentParams>((yysemantic_stack_[(6) - (5)].componentParams)));								}
    break;

  case 45:


#line 222 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(2) - (2)].name), ComponentNameSource::strGuid((yysemantic_stack_[(2) - (2)].name)));										}
    break;

  case 46:


#line 223 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(5) - (2)].name), ComponentNameSource::strGuid((yysemantic_stack_[(5) - (2)].name)), std::unique_ptr<ComponentParams>((yysemantic_stack_[(5) - (4)].componentParams)));	}
    break;

  case 48:


#line 228 "parser/parser.yacc"
    {		}
    break;

  case 49:


#line 229 "parser/parser.yacc"
    { yylex.gc.endComponentRuntimeParams(std::unique_ptr<ComponentParams>((yysemantic_stack_[(4) - (3)].componentParams)));	}
    break;

  case 50:


#line 234 "parser/parser.yacc"
    {
		ComponentParams* params = new ComponentParams();
		params->emplace_back((yysemantic_stack_[(1) - (1)].componentParam));
		(yyval.componentParams)=params;
	}
    break;

  case 51:


#line 239 "parser/parser.yacc"
    { (yyval.componentParams)->emplace_back((yysemantic_stack_[(3) - (3)].componentParam)); }
    break;

  case 52:


#line 243 "parser/parser.yacc"
    { ComponentParam* param = new ComponentParam((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].name)); (yyval.componentParam) = param; }
    break;

  case 53:


#line 247 "parser/parser.yacc"
    { (yyval.guidList) = new Expressions::ConstExprList((yysemantic_stack_[(1) - (1)].expr));	}
    break;

  case 54:


#line 248 "parser/parser.yacc"
    { (yysemantic_stack_[(3) - (1)].guidList)->push_back((yysemantic_stack_[(3) - (3)].expr)); (yyval.guidList)=(yysemantic_stack_[(3) - (1)].guidList);					}
    break;

  case 55:


#line 249 "parser/parser.yacc"
    { (yyval.guidList)=(yysemantic_stack_[(2) - (1)].guidList);									}
    break;

  case 56:


#line 253 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 57:


#line 254 "parser/parser.yacc"
    { ENFORCE(true);}
    break;

  case 58:


#line 255 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].proxy);		}
    break;

  case 59:


#line 256 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 60:


#line 257 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 61:


#line 258 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(3) - (2)].expr);		}
    break;

  case 62:


#line 259 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 63:


#line 260 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 64:


#line 261 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 65:


#line 262 "parser/parser.yacc"
    {}
    break;

  case 66:


#line 266 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(3) - (1)].name), nullptr);			}
    break;

  case 67:


#line 267 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 68:


#line 271 "parser/parser.yacc"
    { (yyval.propertyAssignmentList) = yylex.gc.newPropertyAssignmentsList((yysemantic_stack_[(1) - (1)].propertyAssignment));		}
    break;

  case 69:


#line 272 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(2) - (1)].propertyAssignmentList), (yysemantic_stack_[(2) - (2)].propertyAssignment));		}
    break;

  case 70:


#line 273 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(3) - (1)].propertyAssignmentList), (yysemantic_stack_[(3) - (3)].propertyAssignment));		}
    break;

  case 71:


#line 277 "parser/parser.yacc"
    { (yyval.propertyAssignment) = (yysemantic_stack_[(1) - (1)].propertyAssignment);											}
    break;

  case 72:


#line 278 "parser/parser.yacc"
    { (yyval.propertyAssignment) = (yysemantic_stack_[(1) - (1)].propertyAssignment);											}
    break;

  case 73:


#line 279 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(1) - (1)].expr));			}
    break;

  case 74:


#line 280 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(1) - (1)].component));			}
    break;

  case 75:


#line 284 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.addExternalExtension((yysemantic_stack_[(3) - (2)].name), (yysemantic_stack_[(3) - (3)].name) ); }
    break;

  case 76:


#line 288 "parser/parser.yacc"
    {  }
    break;

  case 77:


#line 292 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("less",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 78:


#line 293 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("greater",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 79:


#line 294 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("lequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 80:


#line 295 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("gequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 81:


#line 296 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("equal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 82:


#line 297 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("notequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 83:


#line 298 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("and",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 84:


#line 299 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("or",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 85:


#line 300 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("not",		Expressions::ConstExprList((yysemantic_stack_[(2) - (2)].expr)));		}
    break;

  case 86:


#line 301 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("add", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 87:


#line 302 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("sub", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 88:


#line 303 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mul", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 89:


#line 304 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("div", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 90:


#line 305 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mod", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 91:


#line 309 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(5) - (5)].lambda); }
    break;

  case 92:


#line 310 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(6) - (6)].lambda); yylex.gc.fillLambdaParams((yysemantic_stack_[(6) - (6)].lambda), (yysemantic_stack_[(6) - (4)].guidList)); }
    break;

  case 93:


#line 311 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(6) - (6)].lambda); yylex.gc.fillLambdaCaptures((yysemantic_stack_[(6) - (6)].lambda), (yysemantic_stack_[(6) - (2)].guidList)); }
    break;

  case 94:


#line 312 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(7) - (7)].lambda); yylex.gc.fillLambdaCaptures((yysemantic_stack_[(7) - (7)].lambda), (yysemantic_stack_[(7) - (2)].guidList)); yylex.gc.fillLambdaParams((yysemantic_stack_[(7) - (7)].lambda), (yysemantic_stack_[(7) - (5)].guidList)); }
    break;

  case 95:


#line 316 "parser/parser.yacc"
    {	(yyval.lambda) = yylex.gc.newLambda();		}
    break;

  case 96:


#line 317 "parser/parser.yacc"
    {	(yyval.lambda) = yylex.gc.newLambda((yysemantic_stack_[(3) - (2)].expr));	}
    break;

  case 97:


#line 318 "parser/parser.yacc"
    {	(yyval.lambda) = yylex.gc.newLambda((yysemantic_stack_[(4) - (3)].expr));	}
    break;

  case 98:


#line 322 "parser/parser.yacc"
    {	(yyval.guidList) = (yysemantic_stack_[(3) - (2)].guidList);					}
    break;

  case 99:


#line 323 "parser/parser.yacc"
    {	(yyval.guidList) = new Expressions::ConstExprList();	}
    break;

  case 100:


#line 327 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>()); }
    break;

  case 101:


#line 328 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(3) - (2)].propertyAssignmentList))); }
    break;

  case 102:


#line 333 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(1) - (1)].proxy);}
    break;

  case 103:


#line 334 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(2) - (1)].proxy); (yysemantic_stack_[(2) - (1)].proxy)->addPathElement((yysemantic_stack_[(2) - (2)].proxyPath));	}
    break;

  case 104:


#line 335 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 105:


#line 336 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 106:


#line 340 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy(yylloc);				}
    break;

  case 107:


#line 341 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy("iterator", yylloc);	}
    break;

  case 108:


#line 342 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy((yysemantic_stack_[(1) - (1)].name), yylloc);			}
    break;

  case 109:


#line 343 "parser/parser.yacc"
    { 
																		  (yyval.proxy) = yylex.gc.newProxy(
																					yylex.gc.newProxyCallablePath(
																						yylex.gc.newCallable((yysemantic_stack_[(2) - (1)].name), std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(2) - (2)].guidList)))), yylloc);
																		}
    break;

  case 110:


#line 351 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyArrayPath(std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(3) - (2)].guidList))); }
    break;

  case 111:


#line 355 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyComponentPath((yysemantic_stack_[(3) - (2)].name)); }
    break;

  case 112:


#line 359 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyPropertyPath((yysemantic_stack_[(1) - (1)].name)); }
    break;

  case 113:


#line 364 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].intVal),				yylloc);	}
    break;

  case 114:


#line 365 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(std::string((yysemantic_stack_[(1) - (1)].strVal)),	yylloc);	}
    break;

  case 115:


#line 366 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].fltVal),				yylloc);	}
    break;

  case 116:


#line 367 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(true,				yylloc);	}
    break;

  case 117:


#line 368 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(false,				yylloc);	}
    break;

  case 118:


#line 369 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(3) - (2)].guidList)));}
    break;

  case 119:


#line 370 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>());}
    break;




#line 1194 "parser/parser.cpp"
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


  
  const short int Parser::yypact_ninf_ = -187;
  const short int
  Parser::yypact_[] =
  {
       220,  -187,     4,    26,    46,   178,  -187,  -187,   -28,   150,
    -187,  -187,  -187,   237,    58,   422,    58,    38,    29,  -187,
      92,  -187,  -187,   441,  -187,  -187,  -187,  -187,    52,   100,
      30,  -187,    70,   113,  -187,  -187,   450,   114,    14,   187,
     266,   372,     2,    25,   107,  -187,  -187,   321,    27,   372,
       0,  -187,     5,   456,    58,  -187,   458,   481,   372,    73,
    -187,  -187,  -187,  -187,  -187,   192,  -187,   120,  -187,     1,
    -187,  -187,  -187,  -187,  -187,   372,   496,   372,  -187,   290,
     143,    24,   668,  -187,  -187,  -187,  -187,  -187,  -187,    30,
    -187,   532,   117,  -187,    34,  -187,   372,  -187,   234,   124,
     182,    40,  -187,   190,  -187,  -187,  -187,   104,  -187,   464,
    -187,  -187,   472,  -187,   350,   556,   372,  -187,  -187,   170,
    -187,   372,   372,   499,    16,  -187,   374,   580,   149,    55,
     201,   163,  -187,   372,   372,   372,   372,   372,   372,   372,
     372,   372,   372,   372,   372,   372,   372,   165,   222,   182,
    -187,   668,  -187,   182,   156,  -187,   180,  -187,  -187,  -187,
    -187,   200,   668,  -187,   668,   668,  -187,   393,  -187,  -187,
     319,   183,  -187,   668,   562,   586,   728,   734,   751,   704,
     710,   121,   278,   221,   228,   243,   604,   756,   504,  -187,
    -187,   175,  -187,  -187,   512,  -187,   208,   204,   343,   372,
    -187,   399,  -187,  -187,   405,   216,  -187,   208,   208,   209,
     686,  -187,  -187,   372,  -187,   632,  -187,  -187,   208,   650,
    -187,  -187,  -187
  };

  
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     6,     0,     0,     0,     0,     2,     5,     0,     0,
       4,     1,     3,     0,     0,     0,     0,   108,    47,   106,
       0,   107,     7,     0,    20,    24,    23,    22,    40,     0,
      26,   102,    17,     0,    16,    11,     0,     0,     0,     0,
       0,     0,    25,     0,    35,     8,    21,     0,    45,     0,
       0,   103,     0,     0,     0,    12,     0,     0,     0,   108,
      27,    73,    71,    72,    74,     0,    68,     0,    65,   108,
     114,   115,   113,   116,   117,     0,     0,     0,    99,     0,
       0,     0,    53,    62,    64,    59,    60,    57,    63,    58,
      56,     0,     0,    48,     0,    50,     0,    41,     0,    43,
       0,     0,   112,     0,   104,   105,    18,     0,     9,     0,
      15,    13,     0,    29,     0,     0,     0,   109,    28,     0,
      69,     0,     0,     0,    85,   100,     0,     0,   119,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    36,    42,     0,     0,   110,     0,    19,    10,    14,
      30,     0,    37,    70,    39,    38,    66,     0,   101,    61,
       0,   118,    75,    54,    77,    78,    81,    82,    79,    83,
      84,    86,    87,    89,    90,    88,     0,    80,     0,    52,
      51,     0,    46,   111,     0,    67,     0,     0,     0,     0,
      31,     0,    44,    33,     0,     0,    91,     0,     0,     0,
      76,    32,    34,     0,    95,     0,    92,    93,     0,     0,
      96,    94,    97
  };

  
  const short int
  Parser::yypgoto_[] =
  {
      -187,  -187,   252,  -187,   245,   224,    49,   -12,    12,  -187,
    -187,  -187,    56,  -187,  -187,   -94,   126,   -48,    -5,  -187,
     -43,   -52,  -187,  -187,  -187,  -187,  -186,   259,  -187,   -13,
    -187,  -187,  -187,  -187,  -187
  };

  
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     5,     6,     7,    33,    34,    23,    24,    61,    26,
      62,    63,    64,    28,    29,    94,    95,    81,    82,    83,
      65,    66,    84,    85,    86,    87,   206,   117,    88,    89,
      31,    51,   104,   105,    90
  };

  
  const signed char Parser::yytable_ninf_ = -110;
  const short int
  Parser::yytable_[] =
  {
        30,   101,    30,   102,    98,   -25,   154,     8,    59,   107,
      30,    46,   -25,   120,   114,    18,   -25,   -25,    13,    19,
      14,   216,   217,    30,    46,    25,    67,    25,    92,     9,
      99,   129,   221,   126,    67,    25,    91,   133,   134,    67,
      30,    38,   -25,    30,    67,    21,   120,   123,    25,   -25,
      10,    40,    67,   115,  -109,   120,   106,   103,  -109,   191,
      57,    32,   120,    67,    36,    25,    58,   163,    25,    27,
     124,    27,   127,   131,   120,   132,    38,   100,    93,    27,
     167,    43,    49,   149,    39,    67,    50,   150,    40,   131,
      41,   151,    27,   155,    67,    44,    30,    46,    47,    30,
      46,    67,   109,    48,   131,   112,    67,    59,   171,    27,
      67,   162,    27,    67,    18,   120,   164,   165,    19,    39,
      52,    25,   197,    40,    25,    41,   173,   116,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   133,   134,    21,   201,   130,   121,   122,   120,
     209,   204,   120,   119,    67,   157,   142,   143,   144,    53,
      56,    96,    54,    54,    68,    27,    69,    70,    27,    71,
      72,   148,    49,    59,   153,    67,    50,    19,    11,     1,
      18,    67,    73,    74,    19,    92,     2,     3,    67,     4,
      59,    67,    75,   156,   210,    59,    15,    18,    16,   170,
     215,    19,    18,    21,   172,   149,    19,   192,   219,    76,
      21,   188,   -55,    77,   -55,    79,   -55,    68,    80,    69,
      70,     1,    71,    72,   149,   189,   202,    21,     2,     3,
      19,     4,    21,   198,    60,    73,    74,    59,   193,   118,
      17,   119,   133,   134,    18,    75,   194,    18,    19,   133,
     134,    19,    20,   131,   205,   207,    21,    12,   131,   213,
     218,    37,    76,   214,   133,   134,    77,    68,    79,    69,
      70,    80,    71,    72,    21,   190,    42,    21,   110,     0,
      19,   152,     0,   119,    22,    73,    74,     0,     0,     0,
       0,    68,     0,    69,    70,    75,    71,    72,     0,   133,
     134,     0,     0,     0,    19,     0,    21,     0,     0,    73,
      74,     0,    76,   142,   143,   144,    77,    78,    79,    75,
      68,    80,    69,    70,    59,    71,    72,     0,     0,     0,
      21,    18,     0,    19,     0,    19,    76,     0,    73,    74,
      77,     0,    79,   128,    68,    80,    69,    70,    75,    71,
      72,     0,     0,    59,     0,     0,     0,    19,     0,    21,
      18,    21,    73,    74,    19,    76,     0,     0,    97,    77,
     196,    79,    75,    68,    80,    69,    70,    59,    71,    72,
       0,     0,     0,    21,    18,     0,    19,     0,    19,    76,
      21,    73,    74,    77,   208,    79,    59,   160,    80,   119,
       0,    75,    59,    18,     0,     0,     0,    19,    59,    18,
       0,     0,    21,    19,    21,    18,     0,     0,    76,    19,
       0,   168,    77,   119,    79,    17,     0,    80,     0,     0,
       0,     0,    18,    21,     0,     0,    19,    20,     0,    21,
     195,     0,   119,     0,    17,    21,   211,     0,   119,     0,
       0,    18,   212,    17,   119,    19,    20,     0,     0,    17,
      18,    17,    21,     0,    19,    20,    18,    17,    18,    35,
      19,    20,    19,    20,    18,    17,     0,     0,    19,    20,
       0,    21,    18,     0,    59,     0,    19,    20,    45,     0,
      21,    18,     0,     0,     0,    19,    21,    55,    21,    59,
       0,     0,    59,   108,    21,   111,    18,    59,     0,    18,
      19,   158,    21,    19,    18,    59,     0,     0,    19,   159,
       0,    21,    18,     0,     0,     0,    19,     0,   113,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,    21,
       0,     0,     0,   125,    21,     0,   166,     0,     0,     0,
       0,   200,    21,   133,   134,   135,   136,   137,     0,   203,
       0,     0,   138,   139,     0,   140,   141,   142,   143,   144,
     145,     0,     0,     0,     0,     0,   146,   133,   134,   135,
     136,   137,     0,   133,   134,   147,   138,   139,     0,   140,
     141,   142,   143,   144,   145,   140,   141,   142,   143,   144,
     146,   133,   134,   135,   136,   137,     0,   133,   134,   161,
     138,   139,     0,   140,   141,   142,   143,   144,   145,   140,
     141,   142,   143,   144,   146,   133,   134,   135,   136,   137,
       0,   169,     0,     0,   138,   139,     0,   140,   141,   142,
     143,   144,   145,     0,     0,     0,     0,     0,   146,     0,
       0,     0,   199,   133,   134,   135,   136,   137,     0,     0,
       0,     0,   138,   139,     0,   140,   141,   142,   143,   144,
     145,   133,   134,   135,   136,   137,   146,     0,     0,   220,
     138,   139,     0,   140,   141,   142,   143,   144,   145,   133,
     134,   135,   136,   137,   146,     0,     0,   222,   138,   139,
       0,   140,   141,   142,   143,   144,   145,   133,   134,   135,
     136,   137,   146,     0,     0,     0,   138,   139,     0,   140,
     141,   142,   143,   144,   145,   133,   134,   135,   136,   137,
     146,   133,   134,   135,   136,   137,     0,   140,   141,   142,
     143,   144,     0,   140,   141,   142,   143,   144,   146,   133,
     134,     0,     0,     0,   146,   133,   134,     0,     0,     0,
       0,   140,   141,   142,   143,   144,     0,   140,   141,   142,
     143,   144,   133,   134,     0,     0,     0,   133,   134,     0,
       0,     0,     0,     0,   140,   141,   142,   143,   144,   140,
     141,   142,   143,   144
  };

  
  const short int
  Parser::yycheck_[] =
  {
        13,    49,    15,     3,    47,     3,   100,     3,     3,    52,
      23,    23,    10,    65,    57,    10,    14,    15,    46,    14,
      48,   207,   208,    36,    36,    13,    39,    15,     3,     3,
       3,    79,   218,    76,    47,    23,    41,    21,    22,    52,
      53,     3,    40,    56,    57,    40,    98,    46,    36,    47,
       4,    50,    65,    58,    52,   107,    51,    57,    56,   153,
      46,     3,   114,    76,    15,    53,    52,   119,    56,    13,
      75,    15,    77,    49,   126,    51,     3,    50,    53,    23,
     123,    52,    52,    49,    46,    98,    56,    53,    50,    49,
      52,    96,    36,    53,   107,     3,   109,   109,    46,   112,
     112,   114,    53,     3,    49,    56,   119,     3,    53,    53,
     123,   116,    56,   126,    10,   167,   121,   122,    14,    46,
      50,   109,   170,    50,   112,    52,   131,    54,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    21,    22,    40,   188,     3,    27,    28,   201,
     198,   194,   204,    49,   167,    51,    35,    36,    37,    46,
      46,    54,    49,    49,     1,   109,     3,     4,   112,     6,
       7,    54,    52,     3,    50,   188,    56,    14,     0,     1,
      10,   194,    19,    20,    14,     3,     8,     9,   201,    11,
       3,   204,    29,     3,   199,     3,    46,    10,    48,    50,
     205,    14,    10,    40,     3,    49,    14,    51,   213,    46,
      40,    46,    49,    50,    51,    52,    53,     1,    55,     3,
       4,     1,     6,     7,    49,     3,    51,    40,     8,     9,
      14,    11,    40,    50,    47,    19,    20,     3,    58,    47,
       3,    49,    21,    22,    10,    29,    46,    10,    14,    21,
      22,    14,    15,    49,    46,    51,    40,     5,    49,    43,
      51,    16,    46,    47,    21,    22,    50,     1,    52,     3,
       4,    55,     6,     7,    40,   149,    17,    40,    54,    -1,
      14,    47,    -1,    49,    47,    19,    20,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,    29,     6,     7,    -1,    21,
      22,    -1,    -1,    -1,    14,    -1,    40,    -1,    -1,    19,
      20,    -1,    46,    35,    36,    37,    50,    51,    52,    29,
       1,    55,     3,     4,     3,     6,     7,    -1,    -1,    -1,
      40,    10,    -1,    14,    -1,    14,    46,    -1,    19,    20,
      50,    -1,    52,    53,     1,    55,     3,     4,    29,     6,
       7,    -1,    -1,     3,    -1,    -1,    -1,    14,    -1,    40,
      10,    40,    19,    20,    14,    46,    -1,    -1,    47,    50,
      51,    52,    29,     1,    55,     3,     4,     3,     6,     7,
      -1,    -1,    -1,    40,    10,    -1,    14,    -1,    14,    46,
      40,    19,    20,    50,    51,    52,     3,    47,    55,    49,
      -1,    29,     3,    10,    -1,    -1,    -1,    14,     3,    10,
      -1,    -1,    40,    14,    40,    10,    -1,    -1,    46,    14,
      -1,    47,    50,    49,    52,     3,    -1,    55,    -1,    -1,
      -1,    -1,    10,    40,    -1,    -1,    14,    15,    -1,    40,
      47,    -1,    49,    -1,     3,    40,    47,    -1,    49,    -1,
      -1,    10,    47,     3,    49,    14,    15,    -1,    -1,     3,
      10,     3,    40,    -1,    14,    15,    10,     3,    10,    47,
      14,    15,    14,    15,    10,     3,    -1,    -1,    14,    15,
      -1,    40,    10,    -1,     3,    -1,    14,    15,    47,    -1,
      40,    10,    -1,    -1,    -1,    14,    40,    47,    40,     3,
      -1,    -1,     3,    47,    40,    47,    10,     3,    -1,    10,
      14,    47,    40,    14,    10,     3,    -1,    -1,    14,    47,
      -1,    40,    10,    -1,    -1,    -1,    14,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    40,
      -1,    -1,    -1,    47,    40,    -1,    47,    -1,    -1,    -1,
      -1,    47,    40,    21,    22,    23,    24,    25,    -1,    47,
      -1,    -1,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    -1,    44,    21,    22,    23,
      24,    25,    -1,    21,    22,    53,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    33,    34,    35,    36,    37,
      44,    21,    22,    23,    24,    25,    -1,    21,    22,    53,
      30,    31,    -1,    33,    34,    35,    36,    37,    38,    33,
      34,    35,    36,    37,    44,    21,    22,    23,    24,    25,
      -1,    51,    -1,    -1,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    48,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    21,    22,    23,    24,    25,    44,    -1,    -1,    47,
      30,    31,    -1,    33,    34,    35,    36,    37,    38,    21,
      22,    23,    24,    25,    44,    -1,    -1,    47,    30,    31,
      -1,    33,    34,    35,    36,    37,    38,    21,    22,    23,
      24,    25,    44,    -1,    -1,    -1,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    21,    22,    23,    24,    25,
      44,    21,    22,    23,    24,    25,    -1,    33,    34,    35,
      36,    37,    -1,    33,    34,    35,    36,    37,    44,    21,
      22,    -1,    -1,    -1,    44,    21,    22,    -1,    -1,    -1,
      -1,    33,    34,    35,    36,    37,    -1,    33,    34,    35,
      36,    37,    21,    22,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,    33,
      34,    35,    36,    37
  };

  
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     8,     9,    11,    60,    61,    62,     3,     3,
       4,     0,    61,    46,    48,    46,    48,     3,    10,    14,
      15,    40,    47,    65,    66,    67,    68,    71,    72,    73,
      88,    89,     3,    63,    64,    47,    65,    63,     3,    46,
      50,    52,    86,    52,     3,    47,    66,    46,     3,    52,
      56,    90,    50,    46,    49,    47,    46,    46,    52,     3,
      47,    67,    69,    70,    71,    79,    80,    88,     1,     3,
       4,     6,     7,    19,    20,    29,    46,    50,    51,    52,
      55,    76,    77,    78,    81,    82,    83,    84,    87,    88,
      93,    77,     3,    53,    74,    75,    54,    47,    79,     3,
      50,    76,     3,    57,    91,    92,    51,    79,    47,    65,
      64,    47,    65,    47,    79,    77,    54,    86,    47,    49,
      80,    27,    28,    46,    77,    47,    79,    77,    53,    76,
       3,    49,    51,    21,    22,    23,    24,    25,    30,    31,
      33,    34,    35,    36,    37,    38,    44,    53,    54,    49,
      53,    77,    47,    50,    74,    53,     3,    51,    47,    47,
      47,    53,    77,    80,    77,    77,    47,    79,    47,    51,
      50,    53,     3,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    46,     3,
      75,    74,    51,    58,    46,    47,    51,    76,    50,    48,
      47,    79,    51,    47,    79,    46,    85,    51,    51,    76,
      77,    47,    47,    43,    47,    77,    85,    85,    51,    77,
      47,    85,    47
  };

#if YYDEBUG
  
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   123,   125,    58,    44,
      40,    41,    91,    93,    61,    35,    46,    60,    62
  };
#endif

  
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    59,    60,    60,    61,    61,    61,    62,    62,    62,
      62,    62,    62,    62,    62,    63,    63,    64,    64,    64,
      65,    65,    66,    66,    66,    66,    66,    67,    67,    67,
      67,    67,    67,    67,    67,    68,    68,    69,    70,    70,
      71,    71,    71,    72,    72,    72,    72,    73,    73,    73,
      74,    74,    75,    76,    76,    76,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    78,    78,    79,    79,
      79,    80,    80,    80,    80,    81,    82,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    84,    84,    84,    84,    85,    85,    85,    86,    86,
      87,    87,    88,    88,    88,    88,    89,    89,    89,    89,
      90,    91,    92,    93,    93,    93,    93,    93,    93,    93
  };

  
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     2,     2,     1,     1,     4,     5,     6,
       7,     4,     5,     6,     7,     3,     1,     1,     3,     4,
       1,     2,     1,     1,     1,     2,     1,     3,     4,     4,
       5,     6,     7,     7,     8,     2,     4,     3,     3,     3,
       1,     3,     4,     3,     6,     2,     5,     1,     3,     4,
       1,     3,     3,     1,     3,     2,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     3,     4,     1,     2,
       3,     1,     1,     1,     1,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     3,
       3,     5,     6,     6,     7,     2,     3,     4,     3,     2,
       2,     3,     1,     2,     3,     3,     1,     1,     1,     2,
       3,     3,     1,     1,     1,     1,     1,     1,     3,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  
  const char*
  const Parser::yytname_[] =
  {
    "$end", "error", "$undefined", "literal", "str", "text", "flt", "digit",
  "\"class definition\"", "\"interface def\"", "\"component definition\"",
  "\"include\"", "\"collection definition\"", "instance_key", "\"this\"",
  "PROPERTY", "LAMBDA", "ARRAY", "NOINSTANCE", "true_key", "false_key",
  "LESS", "GREATER", "EQUAL", "NOT_EQUAL", "LESS_EQUAL", "MORE_EQUAL",
  "FORWARD", "BACKWARD", "NOT", "AND", "OR", "XOR", "PLUS", "MINUS", "DIV",
  "MOD", "MUL", "QUERY", "COLON", "ITERATOR", "EXTERNAL", "PRIVATE",
  "\"return\"", "GREATER_EQUAL", "COMPARISON", "'{'", "'}'", "':'", "','",
  "'('", "')'", "'['", "']'", "'='", "'#'", "'.'", "'<'", "'>'", "$accept",
  "file", "file_element", "class_def", "parent_list", "mixInheritance",
  "body", "element", "instance_def", "property_def", "property_assignment",
  "connection", "component", "component_def", "component_runtime_params",
  "component_params", "component_param", "exp_list", "exp", "type_def",
  "instance_body", "instance_element", "external_language", "if_operator",
  "operators", "lambda", "lambda_body", "call_operator", "anonymousStruct",
  "proxy", "proxy_head", "array_path_element", "components_path_element",
  "property_path_element", "const", 0
  };
#endif

#if YYDEBUG
  
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        60,     0,    -1,    61,    -1,    60,    61,    -1,    11,     4,
      -1,    62,    -1,     1,    -1,     8,     3,    46,    47,    -1,
       8,     3,    46,    65,    47,    -1,     8,     3,    48,    63,
      46,    47,    -1,     8,     3,    48,    63,    46,    65,    47,
      -1,     9,     3,    46,    47,    -1,     9,     3,    46,    65,
      47,    -1,     9,     3,    48,    63,    46,    47,    -1,     9,
       3,    48,    63,    46,    65,    47,    -1,    63,    49,    64,
      -1,    64,    -1,     3,    -1,     3,    50,    51,    -1,     3,
      50,    79,    51,    -1,    66,    -1,    65,    66,    -1,    71,
      -1,    68,    -1,    67,    -1,     3,    86,    -1,    88,    -1,
       3,    46,    47,    -1,     3,    46,    79,    47,    -1,     3,
       3,    46,    47,    -1,     3,     3,    46,    79,    47,    -1,
       3,    52,    77,    53,    46,    47,    -1,     3,    52,    77,
      53,    46,    79,    47,    -1,     3,     3,    52,    77,    53,
      46,    47,    -1,     3,     3,    52,    77,    53,    46,    79,
      47,    -1,    15,     3,    -1,    15,     3,    54,    77,    -1,
       3,    54,    77,    -1,    88,    28,    77,    -1,    88,    27,
      77,    -1,    72,    -1,    72,    46,    47,    -1,    72,    46,
      79,    47,    -1,    73,     3,     3,    -1,    73,     3,     3,
      50,    74,    51,    -1,    73,     3,    -1,    73,     3,    50,
      74,    51,    -1,    10,    -1,    10,    52,    53,    -1,    10,
      52,    74,    53,    -1,    75,    -1,    74,    49,    75,    -1,
       3,    54,     3,    -1,    77,    -1,    76,    49,    77,    -1,
      76,    49,    -1,    93,    -1,    84,    -1,    88,    -1,    82,
      -1,    83,    -1,    50,    77,    51,    -1,    78,    -1,    87,
      -1,    81,    -1,     1,    -1,     3,    46,    47,    -1,     3,
      46,    79,    47,    -1,    80,    -1,    79,    80,    -1,    79,
      49,    80,    -1,    69,    -1,    70,    -1,    67,    -1,    71,
      -1,    55,     3,     3,    -1,    77,    38,    77,    48,    77,
      -1,    77,    21,    77,    -1,    77,    22,    77,    -1,    77,
      25,    77,    -1,    77,    44,    77,    -1,    77,    23,    77,
      -1,    77,    24,    77,    -1,    77,    30,    77,    -1,    77,
      31,    77,    -1,    29,    77,    -1,    77,    33,    77,    -1,
      77,    34,    77,    -1,    77,    37,    77,    -1,    77,    35,
      77,    -1,    77,    36,    77,    -1,    52,    53,    50,    51,
      85,    -1,    52,    53,    50,    76,    51,    85,    -1,    52,
      76,    53,    50,    51,    85,    -1,    52,    76,    53,    50,
      76,    51,    85,    -1,    46,    47,    -1,    46,    77,    47,
      -1,    46,    43,    77,    47,    -1,    50,    76,    51,    -1,
      50,    51,    -1,    46,    47,    -1,    46,    79,    47,    -1,
      89,    -1,    88,    90,    -1,    88,    56,    91,    -1,    88,
      56,    92,    -1,    14,    -1,    40,    -1,     3,    -1,     3,
      86,    -1,    52,    76,    53,    -1,    57,     3,    58,    -1,
       3,    -1,     7,    -1,     4,    -1,     6,    -1,    19,    -1,
      20,    -1,    52,    76,    53,    -1,    52,    53,    -1
  };

  
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,    11,    13,    15,    20,    26,
      33,    41,    46,    52,    59,    67,    71,    73,    75,    79,
      84,    86,    89,    91,    93,    95,    98,   100,   104,   109,
     114,   120,   127,   135,   143,   152,   155,   160,   164,   168,
     172,   174,   178,   183,   187,   194,   197,   203,   205,   209,
     214,   216,   220,   224,   226,   230,   233,   235,   237,   239,
     241,   243,   247,   249,   251,   253,   255,   259,   264,   266,
     269,   273,   275,   277,   279,   281,   285,   291,   295,   299,
     303,   307,   311,   315,   319,   323,   326,   330,   334,   338,
     342,   346,   352,   359,   366,   374,   377,   381,   386,   390,
     393,   396,   400,   402,   405,   409,   413,   415,   417,   419,
     422,   426,   430,   432,   434,   436,   438,   440,   442,   446
  };

  
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   143,   143,   144,   148,   149,   150,   154,   155,   156,
     157,   158,   159,   160,   161,   165,   166,   170,   171,   172,
     176,   177,   181,   182,   183,   184,   185,   189,   190,   191,
     192,   193,   194,   195,   196,   200,   201,   205,   209,   210,
     214,   215,   216,   220,   221,   222,   223,   227,   228,   229,
     233,   239,   243,   247,   248,   249,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   266,   267,   271,   272,
     273,   277,   278,   279,   280,   284,   288,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   309,   310,   311,   312,   316,   317,   318,   322,   323,
     327,   328,   333,   334,   335,   336,   340,   341,   342,   343,
     351,   355,   359,   364,   365,   366,   367,   368,   369,   370
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
       2,     2,     2,     2,     2,    55,     2,     2,     2,     2,
      50,    51,     2,     2,    49,     2,    56,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    48,     2,
      57,    54,    58,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
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
  const int Parser::yylast_ = 793;
  const int Parser::yynnts_ = 35;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 11;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 59;

  const unsigned int Parser::yyuser_token_number_max_ = 300;
  const Parser::token_number_type Parser::yyundef_token_ = 2;



#line 1 "[Bison:b4_percent_define_default]"

} 


#line 1944 "parser/parser.cpp"



#line 375 "parser/parser.yacc"

void ObjectParser::Parser::error(const location_type& loc, const std::string& msg)
{
	LOG_ERROR_UNDECORATED(strlocation(loc) << msg);
}

