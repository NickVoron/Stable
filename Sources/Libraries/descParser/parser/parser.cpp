
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
#line 61 "parser/parser.yacc"

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
#line 141 "parser/parser.yacc"
    {	yylex.compiler.loadClassDescInfo( boost::filesystem::path(yylex.fileName).parent_path() / (yysemantic_stack_[(2) - (2)].strVal) );		}
    break;

  case 5:

/* Line 678 of lalr1.cc  */
#line 142 "parser/parser.yacc"
    {	yylex.gc.endClass();	}
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 149 "parser/parser.yacc"
    { yylex.gc.endCollection((yysemantic_stack_[(8) - (2)].name), (yysemantic_stack_[(8) - (4)].name));	}
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 150 "parser/parser.yacc"
    { yylex.gc.endCollection((yysemantic_stack_[(7) - (2)].name), (yysemantic_stack_[(7) - (4)].name));	}
    break;

  case 12:

/* Line 678 of lalr1.cc  */
#line 159 "parser/parser.yacc"
    {	yylex.gc.addCollectionElement((yysemantic_stack_[(2) - (1)].name), (yysemantic_stack_[(2) - (2)].guidList)); }
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 163 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), false);	}
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 164 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), false);	}
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 165 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), false);	}
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 166 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), false);	}
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 167 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(4) - (2)].name), true);	}
    break;

  case 18:

/* Line 678 of lalr1.cc  */
#line 168 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(5) - (2)].name), true);	}
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 169 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(6) - (2)].name), true);	}
    break;

  case 20:

/* Line 678 of lalr1.cc  */
#line 170 "parser/parser.yacc"
    {	yylex.gc.setClassName((yysemantic_stack_[(7) - (2)].name), true);	}
    break;

  case 22:

/* Line 678 of lalr1.cc  */
#line 175 "parser/parser.yacc"
    {	/*yylex.gc.addParent($1);*/		}
    break;

  case 23:

/* Line 678 of lalr1.cc  */
#line 179 "parser/parser.yacc"
    {		}
    break;

  case 24:

/* Line 678 of lalr1.cc  */
#line 180 "parser/parser.yacc"
    {		}
    break;

  case 25:

/* Line 678 of lalr1.cc  */
#line 184 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(2) - (2)].name), nullptr);		}
    break;

  case 26:

/* Line 678 of lalr1.cc  */
#line 185 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(4) - (2)].name), nullptr);		}
    break;

  case 27:

/* Line 678 of lalr1.cc  */
#line 186 "parser/parser.yacc"
    { yylex.gc.addMixInheriatance((yysemantic_stack_[(5) - (2)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(5) - (4)].propertyAssignmentList)));	}
    break;

  case 28:

/* Line 678 of lalr1.cc  */
#line 190 "parser/parser.yacc"
    { yylex.gc.addAggregateInheritance((yysemantic_stack_[(2) - (2)].name));		}
    break;

  case 36:

/* Line 678 of lalr1.cc  */
#line 207 "parser/parser.yacc"
    {	}
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 208 "parser/parser.yacc"
    {	yylex.gc.setInheritanceAssigment((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList)));	}
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 212 "parser/parser.yacc"
    { yylex.gc.newInstance((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (2)].name), nullptr); }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 213 "parser/parser.yacc"
    { yylex.gc.newInstance((yysemantic_stack_[(5) - (1)].name), (yysemantic_stack_[(5) - (2)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(5) - (4)].propertyAssignmentList))); }
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 214 "parser/parser.yacc"
    { yylex.gc.newInstanceArray((yysemantic_stack_[(7) - (1)].name), (yysemantic_stack_[(7) - (2)].name), (yysemantic_stack_[(7) - (4)].expr), nullptr); }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 215 "parser/parser.yacc"
    { yylex.gc.newInstanceArray((yysemantic_stack_[(8) - (1)].name), (yysemantic_stack_[(8) - (2)].name), (yysemantic_stack_[(8) - (4)].expr), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(8) - (7)].propertyAssignmentList))); }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 219 "parser/parser.yacc"
    {	yylex.gc.addvar((yysemantic_stack_[(4) - (2)].name), (yysemantic_stack_[(4) - (4)].expr));	}
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 223 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(2) - (2)].name));		}
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 224 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(4) - (2)].name),(yysemantic_stack_[(4) - (4)].expr));	}
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 225 "parser/parser.yacc"
    {	yylex.gc.addProperty((yysemantic_stack_[(5) - (3)].name),(yysemantic_stack_[(5) - (5)].expr), true); }
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 229 "parser/parser.yacc"
    { (yyval.propertyAssignment) = yylex.gc.newPropertyAssignment((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].expr));			/*внутри gc происходит накопление присвоенных свойств*/	}
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 230 "parser/parser.yacc"
    { (yyval.propertyAssignment) = 0; yylex.gc.bindRuntimeProperty((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (4)].proxy), In);		/*не понятно для чего сейчас нужно, но пока оставил*/	}
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 231 "parser/parser.yacc"
    { (yyval.propertyAssignment) = 0; yylex.gc.bindRuntimeProperty((yysemantic_stack_[(4) - (1)].name), (yysemantic_stack_[(4) - (4)].proxy), Out);	/*не понятно для чего сейчас нужно, но пока оставил*/	}
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 236 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(1) - (1)].component), nullptr); }
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 237 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(3) - (1)].component), nullptr); }
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 238 "parser/parser.yacc"
    { (yyval.component) = yylex.gc.addComponent((yysemantic_stack_[(4) - (1)].component), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 242 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(3) - (2)].name), (yysemantic_stack_[(3) - (3)].name));																		}
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 243 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(6) - (2)].name), (yysemantic_stack_[(6) - (3)].name), std::unique_ptr<ComponentParams>((yysemantic_stack_[(6) - (5)].componentParams)));								}
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 244 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(2) - (2)].name), str::stringize((yysemantic_stack_[(2) - (2)].name), "_", ComponentNameSource::strGuid()));											}
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 245 "parser/parser.yacc"
    {	(yyval.component) = yylex.gc.newComponent((yysemantic_stack_[(5) - (2)].name), str::stringize((yysemantic_stack_[(5) - (2)].name), "_", ComponentNameSource::strGuid()), std::unique_ptr<ComponentParams>((yysemantic_stack_[(5) - (4)].componentParams)));	}
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 250 "parser/parser.yacc"
    {		}
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 251 "parser/parser.yacc"
    { yylex.gc.endComponentRuntimeParams(std::unique_ptr<ComponentParams>((yysemantic_stack_[(4) - (3)].componentParams)));	}
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 256 "parser/parser.yacc"
    {
		ComponentParams* params = new ComponentParams();
		params->emplace_back((yysemantic_stack_[(1) - (1)].componentParam));
		(yyval.componentParams)=params;
	}
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 261 "parser/parser.yacc"
    { (yyval.componentParams)->emplace_back((yysemantic_stack_[(3) - (3)].componentParam)); }
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 266 "parser/parser.yacc"
    {	
		ComponentParam* param = new ComponentParam((yysemantic_stack_[(3) - (1)].name), (yysemantic_stack_[(3) - (3)].name));
		(yyval.componentParam) = param;
	}
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 273 "parser/parser.yacc"
    { (yyval.guidList) = new Expressions::ConstExprList((yysemantic_stack_[(1) - (1)].expr)); }
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 274 "parser/parser.yacc"
    { (yysemantic_stack_[(3) - (1)].guidList)->push_back((yysemantic_stack_[(3) - (3)].expr)); (yyval.guidList)=(yysemantic_stack_[(3) - (1)].guidList);		}
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 278 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 279 "parser/parser.yacc"
    {	ENFORCE(true);	}
    break;

  case 66:

/* Line 678 of lalr1.cc  */
#line 280 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].proxy);		}
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 281 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 282 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 283 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(3) - (2)].expr);		}
    break;

  case 70:

/* Line 678 of lalr1.cc  */
#line 284 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 71:

/* Line 678 of lalr1.cc  */
#line 285 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 72:

/* Line 678 of lalr1.cc  */
#line 286 "parser/parser.yacc"
    {	(yyval.expr) = (yysemantic_stack_[(1) - (1)].expr);		}
    break;

  case 73:

/* Line 678 of lalr1.cc  */
#line 287 "parser/parser.yacc"
    {}
    break;

  case 74:

/* Line 678 of lalr1.cc  */
#line 291 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(3) - (1)].name), nullptr); }
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 292 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPrototype((yysemantic_stack_[(4) - (1)].name), std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(4) - (3)].propertyAssignmentList))); }
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 296 "parser/parser.yacc"
    { (yyval.propertyAssignmentList) = yylex.gc.newPropertyAssignmentsList((yysemantic_stack_[(1) - (1)].propertyAssignment));	   }
    break;

  case 77:

/* Line 678 of lalr1.cc  */
#line 297 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(2) - (1)].propertyAssignmentList), (yysemantic_stack_[(2) - (2)].propertyAssignment)); }
    break;

  case 78:

/* Line 678 of lalr1.cc  */
#line 298 "parser/parser.yacc"
    { (yyval.propertyAssignmentList)= yylex.gc.addPropertyAssignments((yysemantic_stack_[(3) - (1)].propertyAssignmentList), (yysemantic_stack_[(3) - (3)].propertyAssignment)); }
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 303 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.addExternalExtension((yysemantic_stack_[(2) - (2)].name), std::string() ); }
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 307 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newTernaryOperator((yysemantic_stack_[(5) - (1)].expr), (yysemantic_stack_[(5) - (3)].expr), (yysemantic_stack_[(5) - (5)].expr)); }
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 311 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("less",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 312 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("greater",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 313 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("lequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 314 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("gequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 315 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("equal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 316 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("notequal",	Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 87:

/* Line 678 of lalr1.cc  */
#line 317 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("and",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 88:

/* Line 678 of lalr1.cc  */
#line 318 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("or",		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 89:

/* Line 678 of lalr1.cc  */
#line 319 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("not",		Expressions::ConstExprList((yysemantic_stack_[(2) - (2)].expr)));		}
    break;

  case 90:

/* Line 678 of lalr1.cc  */
#line 320 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("add", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 91:

/* Line 678 of lalr1.cc  */
#line 321 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("sub", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 92:

/* Line 678 of lalr1.cc  */
#line 322 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mul", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 93:

/* Line 678 of lalr1.cc  */
#line 323 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("div", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 94:

/* Line 678 of lalr1.cc  */
#line 324 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunction("mod", 		Expressions::ConstExprList((yysemantic_stack_[(3) - (1)].expr), (yysemantic_stack_[(3) - (3)].expr)));	}
    break;

  case 95:

/* Line 678 of lalr1.cc  */
#line 328 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newLambda((yysemantic_stack_[(4) - (3)].name), *(yysemantic_stack_[(4) - (4)].guidList));	}
    break;

  case 96:

/* Line 678 of lalr1.cc  */
#line 332 "parser/parser.yacc"
    {	(yyval.guidList) = (yysemantic_stack_[(3) - (2)].guidList);					}
    break;

  case 97:

/* Line 678 of lalr1.cc  */
#line 333 "parser/parser.yacc"
    {	(yyval.guidList) = new Expressions::ConstExprList();	}
    break;

  case 98:

/* Line 678 of lalr1.cc  */
#line 338 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newPropertiesStruct("", std::unique_ptr<PropertyAssignmentList>((yysemantic_stack_[(3) - (2)].propertyAssignmentList))); }
    break;

  case 99:

/* Line 678 of lalr1.cc  */
#line 343 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(1) - (1)].proxy);}
    break;

  case 100:

/* Line 678 of lalr1.cc  */
#line 344 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(2) - (1)].proxy); (yysemantic_stack_[(2) - (1)].proxy)->addPathElement((yysemantic_stack_[(2) - (2)].proxyPath));	}
    break;

  case 101:

/* Line 678 of lalr1.cc  */
#line 345 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 102:

/* Line 678 of lalr1.cc  */
#line 346 "parser/parser.yacc"
    { (yyval.proxy) = (yysemantic_stack_[(3) - (1)].proxy); (yysemantic_stack_[(3) - (1)].proxy)->addPathElement((yysemantic_stack_[(3) - (3)].proxyPath));	}
    break;

  case 103:

/* Line 678 of lalr1.cc  */
#line 350 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy();					}
    break;

  case 104:

/* Line 678 of lalr1.cc  */
#line 351 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy("iterator");		}
    break;

  case 105:

/* Line 678 of lalr1.cc  */
#line 352 "parser/parser.yacc"
    { (yyval.proxy) = yylex.gc.newProxy((yysemantic_stack_[(1) - (1)].name));				}
    break;

  case 106:

/* Line 678 of lalr1.cc  */
#line 356 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyArrayPath((yysemantic_stack_[(3) - (2)].intVal)); }
    break;

  case 107:

/* Line 678 of lalr1.cc  */
#line 360 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyComponentPath((yysemantic_stack_[(3) - (2)].name)); }
    break;

  case 108:

/* Line 678 of lalr1.cc  */
#line 364 "parser/parser.yacc"
    { (yyval.proxyPath) = yylex.gc.newProxyPropertyPath((yysemantic_stack_[(1) - (1)].name)); }
    break;

  case 109:

/* Line 678 of lalr1.cc  */
#line 369 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newFunctionOrStruct((yysemantic_stack_[(2) - (1)].name), std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(2) - (2)].guidList)));		}
    break;

  case 110:

/* Line 678 of lalr1.cc  */
#line 370 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].intVal));						}
    break;

  case 111:

/* Line 678 of lalr1.cc  */
#line 371 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(std::string((yysemantic_stack_[(1) - (1)].strVal)));			}
    break;

  case 112:

/* Line 678 of lalr1.cc  */
#line 372 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst((yysemantic_stack_[(1) - (1)].fltVal));						}
    break;

  case 113:

/* Line 678 of lalr1.cc  */
#line 373 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(true);						}
    break;

  case 114:

/* Line 678 of lalr1.cc  */
#line 374 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newConst(false);					}
    break;

  case 115:

/* Line 678 of lalr1.cc  */
#line 375 "parser/parser.yacc"
    { (yyval.expr) = yylex.gc.newArray(std::unique_ptr<Expressions::ConstExprList>((yysemantic_stack_[(3) - (2)].guidList)));	 					}
    break;



/* Line 678 of lalr1.cc  */
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

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
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

  /* YYPGOTO[NTERM-NUM].  */
  const short int
  Parser::yypgoto_[] =
  {
      -128,  -128,   290,  -128,  -128,   210,  -128,   279,   240,  -128,
    -128,   -12,   -26,  -128,  -128,  -128,  -128,   -65,  -128,  -128,
    -128,   -24,   201,   167,   -64,  -128,   -38,  -128,  -128,  -128,
    -128,  -127,  -128,    32,  -128,  -128,  -128,  -128,  -128
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const short int
  Parser::yydefgoto_[] =
  {
        -1,     6,     7,     8,   134,   135,     9,    38,    39,    40,
      41,    27,    28,    29,    30,    31,    32,    67,    33,    34,
      35,    71,    72,   144,   145,   104,    68,   105,   106,   107,
     108,   139,   109,   110,   111,   164,   205,   206,   112
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
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

  /* YYCHECK.  */
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

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
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
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
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

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
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

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
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
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
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
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
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

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
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

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
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


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // ObjectParser

/* Line 1054 of lalr1.cc  */
#line 1795 "parser/parser.cpp"


/* Line 1056 of lalr1.cc  */
#line 380 "parser/parser.yacc"

void ObjectParser::Parser::error(const location_type& loc, const std::string& msg)
{
	LOG_ERROR("parser error in file \"" <<  yylex.fileName << "\" at: "<< loc << msg);
}

