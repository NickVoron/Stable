#ifndef _LEXER_DERIVED_CLASS_H_
#define _LEXER_DERIVED_CLASS_H_
#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#include <FlexLexer.h>
#endif
#include <iostream>
#include <string>

#include "parser.h"


namespace ObjectParser
{
	class Compiler;
	
	class Lexer: public yyFlexLexer 
	{
	public:	 
		Lexer(Compiler& compiler, boost::filesystem::path& file, std::ostream* out = 0);
		Lexer(Compiler& compiler, const std::string& text, std::ostream* out = 0);
		
		Parser::token_type operator()(Parser::semantic_type* lval, Parser::location_type* lloc=0);

		void LexerError(const char* msg);
		void printError(const char* msg, const Parser::location_type& yyloc);

		std::string fileName;
		GrammarComposition& gc;
		Compiler& compiler;

		

	private:
		int yylex();
		Parser::semantic_type* yylval;
		Parser::location_type* yylloc;
		
		std::istringstream input;
	};

	std::string strlocation(const Parser::location_type& loc);
	
}

#endif