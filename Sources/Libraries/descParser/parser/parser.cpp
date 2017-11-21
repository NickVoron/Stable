
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

// Take the name prefix into account.
#define yylex   ObjectParserlex

/* First part of user declarations.  */


/* Line 311 of lalr1.cc  */
#line 43 "parser/parser.cpp"


#include "parser.h"

/* User implementation prologue.  */

/* Line 317 of lalr1.cc  */
#line 63 "parser/parser.yacc"

	#include "lexer.h"
	#include <iostream>
	#include "../compiler/compiler.h"


/* Line 317 of lalr1.cc  */
#line 59 "parser/parser.cpp"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
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

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace ObjectParser {

/* Line 380 of lalr1.cc  */
#line 128 "parser/parser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
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
              /* Fall through.  */
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

  /// Build a parser object.
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
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

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
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 553 of lalr1.cc  */
#line 8 "parser/parser.yacc"
{
	yylloc.initialize(&yylex.fileName);
}

/* Line 553 of lalr1.cc  */
#line 311 "parser/parser.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
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

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
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

/* Line 678 of lalr1.cc  */
#line 146 "parser/parser.yacc"
    {	yylex.compiler.loadClassDescInfo( boost::filesystem::path(yylex.fileName).parent_path() / (yysemantic_stack_[(2) - (2)].strVal) );		}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 147 "parser/parser.yacc"
    {	yylex.gc.endClass();	}
    break;

  case 7:

/* Line 678 of lalr1.cc  */
#line 152 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), false);	}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 153 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), false);	}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 154 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), false);	}
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 155 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), false);	}
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 156 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), true);	}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 157 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), true);	}
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 158 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), true);	}
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 159 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), true);	}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 164 "parser/parser.yacc"
    {	/*yylex.gc.addParent($1);*/		}
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 168 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(1) - (1)].name), nullptr);		}
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 169 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(3) - (1)].name), nullptr);		}
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 170 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList)));	}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 182 "parser/parser.yacc"
    { yylex.gc.addProperty( yylex.gc.newCallable((yysemantic_stack_[(2) - (1)].name), std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(2) - (2)].guidList))));	}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 183 "parser/parser.yacc"
    { yylex.gc.addProperty( (yysemantic_stack_[(1) - (1)].proxy) );						}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 187 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(3) - (1)].name), nullptr); }
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 188 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 29:

/* Line 678 of lalr1.cc  */
#line 189 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (2)].name), nullptr); }
    break;

  case 30:

/* Line 678 of lalr1.cc  */
#line 190 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstance((yysemantic_stack_[(5) - (1)].name), (yysemantic_stack_[(5) - (2)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(5) - (4)].propertyAssignmentList))); }
    break;

  case 31:

/* Line 678 of lalr1.cc  */
#line 191 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(6) - (1)].name), (yysemantic_stack_[(6) - (3)].expr), nullptr); }
    break;

  case 32:

/* Line 678 of lalr1.cc  */
#line 192 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(7) - (1)].name), (yysemantic_stack_[(7) - (3)].expr), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(7) - (6)].propertyAssignmentList))); }
    break;

  case 33:

/* Line 678 of lalr1.cc  */
#line 193 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(7) - (1)].name), (yysemantic_stack_[(7) - (2)].name), (yysemantic_stack_[(7) - (4)].expr), nullptr); }
    break;

  case 34:

/* Line 678 of lalr1.cc  */
#line 194 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newInstanceArray((yysemantic_stack_[(8) - (1)].name), (yysemantic_stack_[(8) - (2)].name), (yysemantic_stack_[(8) - (4)].expr), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(8) - (7)].propertyAssignmentList))); }
    break;

  case 35:

/* Line 678 of lalr1.cc  */
#line 198 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(2) - (2)].name));		}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 199 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(4) - (2)].name), (yysemantic_stack_[(4) - (4)].expr));	}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 203 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].expr));			}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 207 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(1) - (1)].component), nullptr); }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 208 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(3) - (1)].component), nullptr); }
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 209 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(4) - (1)].component), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 213 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(3) - (2)].name), (yysemantic_stack_[(3) - (3)].name));																		}
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 214 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(6) - (2)].name), (yysemantic_stack_[(6) - (3)].name), std::unique_ptr<ComponentParams>((yysemantic_stack_[(6) - (5)].componentParams)));								}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 215 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(2) - (2)].name), ComponentNameSource::strGuid((yysemantic_stack_[(2) - (2)].name)));										}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 216 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(5) - (2)].name), ComponentNameSource::strGuid((yysemantic_stack_[(5) - (2)].name)), std::unique_ptr<ComponentParams>((yysemantic_stack_[(5) - (4)].componentParams)));	}
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 221 "parser/parser.yacc"
    {		}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 222 "parser/parser.yacc"
    { yylex.gc.endComponentRuntimeParams(std::unique_ptr<ComponentParams>((yysemantic_stack_[(4) - (3)].componentParams)));	}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 227 "parser/parser.yacc"
    {
		ComponentParams* params = new ComponentParams();
		params->emplace_back((yysemantic_stack_[(1) - (1)].componentParam));
		(yyval.componentParams)=params;
	}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 232 "parser/parser.yacc"
    { (yyval.componentParams)->emplace_back((yysemantic_stack_[(3) - (3)].componentParam)); }
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 236 "parser/parser.yacc"
    { ComponentParam* param = new ComponentParam((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].name)); (yyval.componentParam) = param; }
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 240 "parser/parser.yacc"
    { (yyval.guidList) = new Expressions::ConstExprList((yysemantic_stack_[(1) - (1)].expr));	}
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 241 "parser/parser.yacc"
    { (yysemantic_stack_[(3) - (1)].guidList)->push_back((yysemantic_stack_[(3) - (3)].expr)); (yyval.guidList)=(yysemantic_stack_[(3) - (1)].guidList);					}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 242 "parser/parser.yacc"
    { (yyval.guidList)=(yysemantic_stack_[(2) - (1)].guidList);									}
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 246 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 247 "parser/parser.yacc"
    { ENFORCE(true);}
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 248 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].proxy);		}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 249 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 250 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 251 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(3) - (2)].expr);		}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 252 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 253 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 254 "parser/parser.yacc"
    { (yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 255 "parser/parser.yacc"
    {}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 259 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(3) - (1)].name), nullptr);			}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 260 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 66:

/* Line 678 of lalr1.cc  */
#line 264 "parser/parser.yacc"
    { (yyval.propertyAssignmentList) = yylex.gc.newPropertyAssignmentsList((yysemantic_stack_[(1) - (1)].propertyAssignment));		}
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 265 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(2) - (1)].propertyAssignmentList), (yysemantic_stack_[(2) - (2)].propertyAssignment));		}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 266 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(3) - (1)].propertyAssignmentList), (yysemantic_stack_[(3) - (3)].propertyAssignment));		}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 270 "parser/parser.yacc"
    { (yyval.propertyAssignment) = (yysemantic_stack_[(1) - (1)].propertyAssignment);											}
    break;

  case 70:

/* Line 678 of lalr1.cc  */
#line 271 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(1) - (1)].expr));			}
    break;

  case 71:

/* Line 678 of lalr1.cc  */
#line 272 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(1) - (1)].component));			}
    break;

  case 72:

/* Line 678 of lalr1.cc  */
#line 278 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.addExternalExtension((yysemantic_stack_[(3) - (2)].name), (yysemantic_stack_[(3) - (3)].name) ); }
    break;

  case 73:

/* Line 678 of lalr1.cc  */
#line 282 "parser/parser.yacc"
    { /*$$ = yylex.gc.newTernaryOperator($1, $3, $5);*/ }
    break;

  case 74:

/* Line 678 of lalr1.cc  */
#line 286 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("less",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 287 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("greater",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 288 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("lequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 77:

/* Line 678 of lalr1.cc  */
#line 289 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("gequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 78:

/* Line 678 of lalr1.cc  */
#line 290 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("equal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 291 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("notequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 292 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("and",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 293 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("or",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 294 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("not",		Expressions::ConstExprList((yysemantic_stack_[(2) - (2)].expr)));		}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 295 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("add", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 296 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("sub", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 297 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mul", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 298 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("div", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 299 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mod", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 303 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(5) - (5)].lambda); }
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 304 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(6) - (6)].lambda); yylex.gc.fillLambdaParams((yysemantic_stack_[(6) - (6)].lambda), (yysemantic_stack_[(6) - (4)].guidList)); }
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 305 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(6) - (6)].lambda); yylex.gc.fillLambdaCaptures((yysemantic_stack_[(6) - (6)].lambda), (yysemantic_stack_[(6) - (2)].guidList)); }
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 306 "parser/parser.yacc"
    { (yyval.expr)=(yysemantic_stack_[(7) - (7)].lambda); yylex.gc.fillLambdaCaptures((yysemantic_stack_[(7) - (7)].lambda), (yysemantic_stack_[(7) - (2)].guidList)); yylex.gc.fillLambdaParams((yysemantic_stack_[(7) - (7)].lambda), (yysemantic_stack_[(7) - (5)].guidList)); }
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 310 "parser/parser.yacc"
    {	(yyval.lambda) = yylex.gc.newLambda();		}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 311 "parser/parser.yacc"
    {	(yyval.lambda) = yylex.gc.newLambda((yysemantic_stack_[(3) - (2)].expr));	}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 312 "parser/parser.yacc"
    {	(yyval.lambda) = yylex.gc.newLambda((yysemantic_stack_[(4) - (3)].expr));	}
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 316 "parser/parser.yacc"
    {	(yyval.guidList) = (yysemantic_stack_[(3) - (2)].guidList);					}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 317 "parser/parser.yacc"
    {	(yyval.guidList) = new Expressions::ConstExprList();	}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 321 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>()); }
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 322 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(3) - (2)].propertyAssignmentList))); }
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 327 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(1) - (1)].proxy);}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 328 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(2) - (1)].proxy); (yysemantic_stack_[(2) - (1)].proxy)->addPathElement((yysemantic_stack_[(2) - (2)].proxyPath));	}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 329 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 330 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 334 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy(yylloc);				}
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 335 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy("iterator", yylloc);	}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 336 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy((yysemantic_stack_[(1) - (1)].name), yylloc);			}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 337 "parser/parser.yacc"
    { 
																		  (yyval.proxy) = yylex.gc.newProxy(
																					yylex.gc.newProxyCallablePath(
																						yylex.gc.newCallable((yysemantic_stack_[(2) - (1)].name), std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(2) - (2)].guidList)))), yylloc);
																		}
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 345 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyArrayPath(std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(3) - (2)].guidList))); }
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 349 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyComponentPath((yysemantic_stack_[(3) - (2)].name)); }
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 353 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyPropertyPath((yysemantic_stack_[(1) - (1)].name)); }
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 358 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].intVal),				yylloc);	}
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 359 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(std::string((yysemantic_stack_[(1) - (1)].strVal)),	yylloc);	}
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 360 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].fltVal),				yylloc);	}
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 361 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(true,				yylloc);	}
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 362 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(false,				yylloc);	}
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 363 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(3) - (2)].guidList)));}
    break;

  case 116:

/* Line 678 of lalr1.cc  */
#line 364 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>());}
    break;



/* Line 678 of lalr1.cc  */
#line 1173 "parser/parser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

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

	/* Pop the current state because it cannot handle the error token.  */
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
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
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

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
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


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Parser::yypact_ninf_ = -89;
  const short int
  Parser::yypact_[] =
  {
       254,   -89,     8,    20,    38,   142,   -89,   -89,    80,    92,
     -89,   -89,   -89,   158,    54,   329,    54,    33,    21,   -89,
      82,   -89,   -89,   355,   -89,   -89,   -89,   -89,    63,    99,
     -10,   -89,    75,    68,   -89,   -89,   361,    74,   -31,   207,
     226,   334,   145,    -1,    77,   -89,   -89,   248,     4,   334,
       0,   -89,    18,   376,    54,   -89,   382,   275,   334,    30,
     -89,   -89,   -89,   -89,    59,   -89,   -89,    -3,   -89,   -89,
     -89,   -89,   -89,   334,   286,   334,   -89,   253,   132,   139,
     559,   -89,   -89,   -89,   -89,   -89,   -89,   -10,   -89,   423,
     105,   -89,    19,   -89,   334,   -89,   117,   115,   167,    25,
     -89,   205,   -89,   -89,   -89,     2,   -89,   395,   -89,   -89,
     398,   -89,   122,   447,   334,   -89,     5,   -89,   302,   -89,
      57,   -89,   146,   471,   164,    58,   218,   127,   -89,   334,
     334,   334,   334,   334,   334,   334,   334,   334,   334,   334,
     334,   334,   334,   180,   234,   167,   -89,   559,   -89,   167,
     140,   -89,   183,   -89,   -89,   -89,   -89,   199,   559,   -89,
     -89,   191,   -89,   -89,   280,   193,   -89,   559,   397,   621,
     628,   636,   643,   591,   606,   404,   428,   213,   247,   294,
     493,   651,   349,   -89,   -89,   155,   -89,   -89,   370,   -89,
     217,   162,   307,   334,   -89,   197,   -89,   -89,   203,   178,
     -89,   217,   217,   171,   575,   -89,   -89,   334,   -89,   509,
     -89,   -89,   217,   534,   -89,   -89,   -89
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  Parser::yydefact_[] =
  {
         0,     6,     0,     0,     0,     0,     2,     5,     0,     0,
       4,     1,     3,     0,     0,     0,     0,   105,    45,   103,
       0,   104,     7,     0,    20,    24,    23,    22,    38,     0,
      26,    99,    17,     0,    16,    11,     0,     0,     0,     0,
       0,     0,    25,     0,    35,     8,    21,     0,    43,     0,
       0,   100,     0,     0,     0,    12,     0,     0,     0,     0,
      27,    70,    69,    71,     0,    66,    63,   105,   111,   112,
     110,   113,   114,     0,     0,     0,    96,     0,     0,     0,
      51,    60,    62,    57,    58,    55,    61,    56,    54,     0,
       0,    46,     0,    48,     0,    39,     0,    41,     0,     0,
     109,     0,   101,   102,    18,     0,     9,     0,    15,    13,
       0,    29,     0,     0,     0,    28,     0,    67,     0,   106,
      82,    97,     0,     0,   116,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    36,    40,     0,
       0,   107,     0,    19,    10,    14,    30,     0,    37,    68,
      64,     0,    98,    59,     0,   115,    72,    52,    74,    75,
      78,    79,    76,    80,    81,    83,    84,    86,    87,    85,
       0,    77,     0,    50,    49,     0,    44,   108,     0,    65,
       0,     0,     0,     0,    31,     0,    42,    33,     0,     0,
      88,     0,     0,     0,    73,    32,    34,     0,    92,     0,
      89,    90,     0,     0,    93,    91,    94
  };

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
       -89,   -89,   242,   -89,   233,   212,    31,     1,     3,   -89,
     -89,     7,   -89,   -89,   -88,   126,   -48,   -41,   -89,   -43,
     -58,   -89,   -89,   -89,   -89,    13,   210,   -89,    12,   -89,
     -89,   -89,   -89,   -89
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     5,     6,     7,    33,    34,    23,    24,    61,    26,
      62,    63,    28,    29,    92,    93,    79,    80,    81,    64,
      65,    82,    83,    84,    85,   200,    42,    86,    87,    31,
      51,   102,   103,    88
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char Parser::yytable_ninf_ = -107;
  const short int
  Parser::yytable_[] =
  {
        89,    99,    90,   100,    96,    59,   117,    97,    59,   105,
     150,     8,    18,    57,   112,    18,    25,   113,    25,    58,
      27,    59,    27,     9,    46,    30,    25,    30,    18,   125,
      27,   122,   120,    38,   123,    30,    38,    46,   117,    25,
      49,   118,    10,    27,    50,    40,    36,   117,    30,   116,
      91,   153,    98,   147,   117,   101,    25,    32,   159,    25,
      27,   185,    59,    27,   117,    30,   145,   104,    30,    18,
     146,    43,   127,   158,    39,   161,   151,    39,   129,   130,
      41,    40,   114,    41,   107,    44,   167,   110,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,    48,   117,   115,   127,   116,    47,    46,   165,
      25,    46,    53,    25,    27,    54,   191,    27,    56,    30,
      59,    54,    30,    52,    13,    59,    14,    18,    66,    94,
      67,    68,    18,    69,    70,   126,    15,   117,    16,   195,
     117,    19,    11,     1,   203,   198,    71,    72,   -25,    59,
       2,     3,   204,     4,    73,   -25,    18,   144,   209,   -25,
     -25,    17,   148,   149,   116,    21,   213,   156,    18,   116,
      90,    74,    19,    20,   -53,    75,   -53,    77,   -53,    66,
      78,    67,    68,   -25,    69,    70,   127,   145,   128,   186,
     -25,   162,    19,   116,    59,  -106,    21,    71,    72,  -106,
      59,    18,   145,    22,   196,    73,    59,    18,   152,   127,
      59,   201,   164,    18,   210,   211,    21,    18,   127,   207,
     212,   166,    74,   208,   182,   215,    75,    66,    77,    67,
      68,    78,    69,    70,   129,   130,   189,   183,   116,   187,
      19,   192,   205,   188,   116,    71,    72,    12,   206,    37,
     116,    59,    60,    73,    66,     1,    67,    68,    18,    69,
      70,   199,     2,     3,    21,     4,   108,    19,   129,   130,
      74,   184,    71,    72,    75,    76,    77,   119,    59,    78,
      73,    66,     0,    67,    68,    18,    69,    70,     0,    59,
       0,    21,     0,    95,    19,     0,    18,    74,     0,    71,
      72,    75,     0,    77,   124,    59,    78,    73,    66,     0,
      67,    68,    18,    69,    70,   129,   130,     0,    21,     0,
     111,    19,     0,     0,    74,     0,    71,    72,    75,   190,
      77,   121,    17,    78,    73,    66,     0,    67,    68,    18,
      69,    70,     0,    19,    20,    21,     0,   160,    19,     0,
       0,    74,    59,    71,    72,    75,   202,    77,    17,    18,
      78,    73,     0,     0,    17,    18,     0,    21,     0,    19,
      20,    18,    21,    59,    35,    19,    20,     0,    74,    17,
      18,     0,    75,     0,    77,    17,    18,    78,     0,     0,
      19,    20,    18,    21,   194,     0,    19,    20,    17,    21,
      45,    17,     0,     0,     0,    18,    55,     0,    18,    19,
      20,     0,    19,    20,    21,   197,     0,     0,   129,   130,
      21,   106,     0,     0,     0,   129,   130,   109,   136,   137,
     138,   139,   140,    21,     0,     0,    21,   138,   139,   140,
     154,     0,     0,   155,   129,   130,   131,   132,   133,   129,
     130,   134,   135,     0,   136,   137,   138,   139,   140,   141,
       0,   138,   139,   140,     0,   142,     0,     0,   129,   130,
     131,   132,   133,     0,   143,   134,   135,     0,   136,   137,
     138,   139,   140,   141,     0,     0,     0,     0,     0,   142,
       0,     0,   129,   130,   131,   132,   133,     0,   157,   134,
     135,     0,   136,   137,   138,   139,   140,   141,     0,     0,
       0,     0,     0,   142,   129,   130,   131,   132,   133,     0,
     163,   134,   135,     0,   136,   137,   138,   139,   140,   141,
     129,   130,   131,   132,   133,   142,     0,   134,   135,   193,
     136,   137,   138,   139,   140,   141,     0,     0,     0,     0,
       0,   142,     0,     0,   214,   129,   130,   131,   132,   133,
       0,     0,   134,   135,     0,   136,   137,   138,   139,   140,
     141,     0,     0,     0,     0,     0,   142,     0,     0,   216,
     129,   130,   131,   132,   133,     0,     0,   134,   135,     0,
     136,   137,   138,   139,   140,   141,   129,   130,   131,   132,
     133,   142,     0,   134,   135,     0,   136,   137,   138,   139,
     140,   141,   129,   130,   131,   132,   133,   142,     0,     0,
       0,     0,   136,   137,   138,   139,   140,   129,   130,   131,
     132,   133,     0,   142,     0,     0,     0,   136,   137,   138,
     139,   140,   129,   130,     0,     0,     0,     0,   142,   129,
     130,     0,   136,   137,   138,   139,   140,   129,   130,   136,
     137,   138,   139,   140,   129,   130,     0,   136,   137,   138,
     139,   140,   129,   130,   136,   137,   138,   139,   140,     0,
       0,     0,   136,   137,   138,   139,   140
  };

  /* YYCHECK.  */
  const short int
  Parser::yycheck_[] =
  {
        41,    49,     3,     3,    47,     3,    64,     3,     3,    52,
      98,     3,    10,    44,    57,    10,    13,    58,    15,    50,
      13,     3,    15,     3,    23,    13,    23,    15,    10,    77,
      23,    74,    73,     3,    75,    23,     3,    36,    96,    36,
      50,    44,     4,    36,    54,    48,    15,   105,    36,    47,
      51,    49,    48,    94,   112,    55,    53,     3,   116,    56,
      53,   149,     3,    56,   122,    53,    47,    49,    56,    10,
      51,    50,    47,   114,    44,   118,    51,    44,    21,    22,
      50,    48,    52,    50,    53,     3,   127,    56,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,     3,   161,    45,    47,    47,    44,   107,    51,
     107,   110,    44,   110,   107,    47,   164,   110,    44,   107,
       3,    47,   110,    48,    44,     3,    46,    10,     1,    52,
       3,     4,    10,     6,     7,     3,    44,   195,    46,   182,
     198,    14,     0,     1,   192,   188,    19,    20,     3,     3,
       8,     9,   193,    11,    27,    10,    10,    52,   199,    14,
      15,     3,    45,    48,    47,    38,   207,    45,    10,    47,
       3,    44,    14,    15,    47,    48,    49,    50,    51,     1,
      53,     3,     4,    38,     6,     7,    47,    47,    49,    49,
      45,    45,    14,    47,     3,    50,    38,    19,    20,    54,
       3,    10,    47,    45,    49,    27,     3,    10,     3,    47,
       3,    49,    48,    10,   201,   202,    38,    10,    47,    41,
      49,     3,    44,    45,    44,   212,    48,     1,    50,     3,
       4,    53,     6,     7,    21,    22,    45,     3,    47,    56,
      14,    48,    45,    44,    47,    19,    20,     5,    45,    16,
      47,     3,    45,    27,     1,     1,     3,     4,    10,     6,
       7,    44,     8,     9,    38,    11,    54,    14,    21,    22,
      44,   145,    19,    20,    48,    49,    50,    67,     3,    53,
      27,     1,    -1,     3,     4,    10,     6,     7,    -1,     3,
      -1,    38,    -1,    45,    14,    -1,    10,    44,    -1,    19,
      20,    48,    -1,    50,    51,     3,    53,    27,     1,    -1,
       3,     4,    10,     6,     7,    21,    22,    -1,    38,    -1,
      45,    14,    -1,    -1,    44,    -1,    19,    20,    48,    49,
      50,    45,     3,    53,    27,     1,    -1,     3,     4,    10,
       6,     7,    -1,    14,    15,    38,    -1,    45,    14,    -1,
      -1,    44,     3,    19,    20,    48,    49,    50,     3,    10,
      53,    27,    -1,    -1,     3,    10,    -1,    38,    -1,    14,
      15,    10,    38,     3,    45,    14,    15,    -1,    44,     3,
      10,    -1,    48,    -1,    50,     3,    10,    53,    -1,    -1,
      14,    15,    10,    38,    45,    -1,    14,    15,     3,    38,
      45,     3,    -1,    -1,    -1,    10,    45,    -1,    10,    14,
      15,    -1,    14,    15,    38,    45,    -1,    -1,    21,    22,
      38,    45,    -1,    -1,    -1,    21,    22,    45,    31,    32,
      33,    34,    35,    38,    -1,    -1,    38,    33,    34,    35,
      45,    -1,    -1,    45,    21,    22,    23,    24,    25,    21,
      22,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      -1,    33,    34,    35,    -1,    42,    -1,    -1,    21,    22,
      23,    24,    25,    -1,    51,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    -1,    21,    22,    23,    24,    25,    -1,    51,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    -1,    42,    21,    22,    23,    24,    25,    -1,
      49,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      21,    22,    23,    24,    25,    42,    -1,    28,    29,    46,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    45,    21,    22,    23,    24,    25,
      -1,    -1,    28,    29,    -1,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    45,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    21,    22,    23,    24,
      25,    42,    -1,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    21,    22,    23,    24,    25,    42,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    21,    22,    23,
      24,    25,    -1,    42,    -1,    -1,    -1,    31,    32,    33,
      34,    35,    21,    22,    -1,    -1,    -1,    -1,    42,    21,
      22,    -1,    31,    32,    33,    34,    35,    21,    22,    31,
      32,    33,    34,    35,    21,    22,    -1,    31,    32,    33,
      34,    35,    21,    22,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    31,    32,    33,    34,    35
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Parser::yystos_[] =
  {
         0,     1,     8,     9,    11,    58,    59,    60,     3,     3,
       4,     0,    59,    44,    46,    44,    46,     3,    10,    14,
      15,    38,    45,    63,    64,    65,    66,    68,    69,    70,
      85,    86,     3,    61,    62,    45,    63,    61,     3,    44,
      48,    50,    83,    50,     3,    45,    64,    44,     3,    50,
      54,    87,    48,    44,    47,    45,    44,    44,    50,     3,
      45,    65,    67,    68,    76,    77,     1,     3,     4,     6,
       7,    19,    20,    27,    44,    48,    49,    50,    53,    73,
      74,    75,    78,    79,    80,    81,    84,    85,    90,    74,
       3,    51,    71,    72,    52,    45,    76,     3,    48,    73,
       3,    55,    88,    89,    49,    76,    45,    63,    62,    45,
      63,    45,    76,    74,    52,    45,    47,    77,    44,    83,
      74,    45,    76,    74,    51,    73,     3,    47,    49,    21,
      22,    23,    24,    25,    28,    29,    31,    32,    33,    34,
      35,    36,    42,    51,    52,    47,    51,    74,    45,    48,
      71,    51,     3,    49,    45,    45,    45,    51,    74,    77,
      45,    76,    45,    49,    48,    51,     3,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    44,     3,    72,    71,    49,    56,    44,    45,
      49,    73,    48,    46,    45,    76,    49,    45,    76,    44,
      82,    49,    49,    73,    74,    45,    45,    41,    45,    74,
      82,    82,    49,    74,    45,    82,    45
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   123,   125,    58,    44,    40,    41,
      91,    93,    61,    35,    46,    60,    62
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Parser::yyr1_[] =
  {
         0,    57,    58,    58,    59,    59,    59,    60,    60,    60,
      60,    60,    60,    60,    60,    61,    61,    62,    62,    62,
      63,    63,    64,    64,    64,    64,    64,    65,    65,    65,
      65,    65,    65,    65,    65,    66,    66,    67,    68,    68,
      68,    69,    69,    69,    69,    70,    70,    70,    71,    71,
      72,    73,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    75,    75,    76,    76,    76,    77,
      77,    77,    78,    79,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    81,    81,
      81,    81,    82,    82,    82,    83,    83,    84,    84,    85,
      85,    85,    85,    86,    86,    86,    86,    87,    88,    89,
      90,    90,    90,    90,    90,    90,    90
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Parser::yyr2_[] =
  {
         0,     2,     1,     2,     2,     1,     1,     4,     5,     6,
       7,     4,     5,     6,     7,     3,     1,     1,     3,     4,
       1,     2,     1,     1,     1,     2,     1,     3,     4,     4,
       5,     6,     7,     7,     8,     2,     4,     3,     1,     3,
       4,     3,     6,     2,     5,     1,     3,     4,     1,     3,
       3,     1,     3,     2,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     3,     4,     1,     2,     3,     1,
       1,     1,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     5,     6,
       6,     7,     2,     3,     4,     3,     2,     2,     3,     1,
       2,     3,     3,     1,     1,     1,     2,     3,     3,     1,
       1,     1,     1,     1,     1,     3,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Parser::yytname_[] =
  {
    "$end", "error", "$undefined", "literal", "str", "text", "flt", "digit",
  "\"class definition\"", "\"interface def\"", "\"component definition\"",
  "\"include\"", "\"collection definition\"", "instance_key", "\"this\"",
  "PROPERTY", "LAMBDA", "ARRAY", "NOINSTANCE", "true_key", "false_key",
  "LESS", "GREATER", "EQUAL", "NOT_EQUAL", "LESS_EQUAL", "MORE_EQUAL",
  "NOT", "AND", "OR", "XOR", "PLUS", "MINUS", "DIV", "MOD", "MUL", "QUERY",
  "COLON", "ITERATOR", "EXTERNAL", "PRIVATE", "\"return\"",
  "GREATER_EQUAL", "COMPARISON", "'{'", "'}'", "':'", "','", "'('", "')'",
  "'['", "']'", "'='", "'#'", "'.'", "'<'", "'>'", "$accept", "file",
  "file_element", "class_def", "parent_list", "mixInheritance", "body",
  "element", "instance_def", "property_def", "property_assignment",
  "component", "component_def", "component_runtime_params",
  "component_params", "component_param", "exp_list", "exp", "type_def",
  "instance_body", "instance_element", "external_language", "if_operator",
  "operators", "lambda", "lambda_body", "call_operator", "anonymousStruct",
  "proxy", "proxy_head", "array_path_element", "components_path_element",
  "property_path_element", "const", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Parser::rhs_number_type
  Parser::yyrhs_[] =
  {
        58,     0,    -1,    59,    -1,    58,    59,    -1,    11,     4,
      -1,    60,    -1,     1,    -1,     8,     3,    44,    45,    -1,
       8,     3,    44,    63,    45,    -1,     8,     3,    46,    61,
      44,    45,    -1,     8,     3,    46,    61,    44,    63,    45,
      -1,     9,     3,    44,    45,    -1,     9,     3,    44,    63,
      45,    -1,     9,     3,    46,    61,    44,    45,    -1,     9,
       3,    46,    61,    44,    63,    45,    -1,    61,    47,    62,
      -1,    62,    -1,     3,    -1,     3,    48,    49,    -1,     3,
      48,    76,    49,    -1,    64,    -1,    63,    64,    -1,    68,
      -1,    66,    -1,    65,    -1,     3,    83,    -1,    85,    -1,
       3,    44,    45,    -1,     3,    44,    76,    45,    -1,     3,
       3,    44,    45,    -1,     3,     3,    44,    76,    45,    -1,
       3,    50,    74,    51,    44,    45,    -1,     3,    50,    74,
      51,    44,    76,    45,    -1,     3,     3,    50,    74,    51,
      44,    45,    -1,     3,     3,    50,    74,    51,    44,    76,
      45,    -1,    15,     3,    -1,    15,     3,    52,    74,    -1,
       3,    52,    74,    -1,    69,    -1,    69,    44,    45,    -1,
      69,    44,    76,    45,    -1,    70,     3,     3,    -1,    70,
       3,     3,    48,    71,    49,    -1,    70,     3,    -1,    70,
       3,    48,    71,    49,    -1,    10,    -1,    10,    50,    51,
      -1,    10,    50,    71,    51,    -1,    72,    -1,    71,    47,
      72,    -1,     3,    52,     3,    -1,    74,    -1,    73,    47,
      74,    -1,    73,    47,    -1,    90,    -1,    81,    -1,    85,
      -1,    79,    -1,    80,    -1,    48,    74,    49,    -1,    75,
      -1,    84,    -1,    78,    -1,     1,    -1,     3,    44,    45,
      -1,     3,    44,    76,    45,    -1,    77,    -1,    76,    77,
      -1,    76,    47,    77,    -1,    67,    -1,    65,    -1,    68,
      -1,    53,     3,     3,    -1,    74,    36,    74,    46,    74,
      -1,    74,    21,    74,    -1,    74,    22,    74,    -1,    74,
      25,    74,    -1,    74,    42,    74,    -1,    74,    23,    74,
      -1,    74,    24,    74,    -1,    74,    28,    74,    -1,    74,
      29,    74,    -1,    27,    74,    -1,    74,    31,    74,    -1,
      74,    32,    74,    -1,    74,    35,    74,    -1,    74,    33,
      74,    -1,    74,    34,    74,    -1,    50,    51,    48,    49,
      82,    -1,    50,    51,    48,    73,    49,    82,    -1,    50,
      73,    51,    48,    49,    82,    -1,    50,    73,    51,    48,
      73,    49,    82,    -1,    44,    45,    -1,    44,    74,    45,
      -1,    44,    41,    74,    45,    -1,    48,    73,    49,    -1,
      48,    49,    -1,    44,    45,    -1,    44,    76,    45,    -1,
      86,    -1,    85,    87,    -1,    85,    54,    88,    -1,    85,
      54,    89,    -1,    14,    -1,    38,    -1,     3,    -1,     3,
      83,    -1,    50,    73,    51,    -1,    55,     3,    56,    -1,
       3,    -1,     7,    -1,     4,    -1,     6,    -1,    19,    -1,
      20,    -1,    50,    73,    51,    -1,    50,    51,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned short int
  Parser::yyprhs_[] =
  {
         0,     0,     3,     5,     8,    11,    13,    15,    20,    26,
      33,    41,    46,    52,    59,    67,    71,    73,    75,    79,
      84,    86,    89,    91,    93,    95,    98,   100,   104,   109,
     114,   120,   127,   135,   143,   152,   155,   160,   164,   166,
     170,   175,   179,   186,   189,   195,   197,   201,   206,   208,
     212,   216,   218,   222,   225,   227,   229,   231,   233,   235,
     239,   241,   243,   245,   247,   251,   256,   258,   261,   265,
     267,   269,   271,   275,   281,   285,   289,   293,   297,   301,
     305,   309,   313,   316,   320,   324,   328,   332,   336,   342,
     349,   356,   364,   367,   371,   376,   380,   383,   386,   390,
     392,   395,   399,   403,   405,   407,   409,   412,   416,   420,
     422,   424,   426,   428,   430,   432,   436
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Parser::yyrline_[] =
  {
         0,   141,   141,   142,   146,   147,   148,   152,   153,   154,
     155,   156,   157,   158,   159,   163,   164,   168,   169,   170,
     174,   175,   179,   180,   181,   182,   183,   187,   188,   189,
     190,   191,   192,   193,   194,   198,   199,   203,   207,   208,
     209,   213,   214,   215,   216,   220,   221,   222,   226,   232,
     236,   240,   241,   242,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   259,   260,   264,   265,   266,   270,
     271,   272,   278,   282,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   303,   304,
     305,   306,   310,   311,   312,   316,   317,   321,   322,   327,
     328,   329,   330,   334,   335,   336,   337,   345,   349,   353,
     358,   359,   360,   361,   362,   363,   364
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
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
       2,     2,     2,     2,     2,    53,     2,     2,     2,     2,
      48,    49,     2,     2,    47,     2,    54,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,     2,
      55,    52,    56,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Parser::yyeof_ = 0;
  const int Parser::yylast_ = 686;
  const int Parser::yynnts_ = 34;
  const int Parser::yyempty_ = -2;
  const int Parser::yyfinal_ = 11;
  const int Parser::yyterror_ = 1;
  const int Parser::yyerrcode_ = 256;
  const int Parser::yyntokens_ = 57;

  const unsigned int Parser::yyuser_token_number_max_ = 298;
  const Parser::token_number_type Parser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // ObjectParser

/* Line 1054 of lalr1.cc  */
#line 1896 "parser/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 369 "parser/parser.yacc"

void ObjectParser::Parser::error(const location_type& loc, const std::string& msg)
{
	LOG_ERROR_UNDECORATED(strlocation(loc) << msg);
}

