#include "lexer.h"
#include "../compiler/compiler.h"

#include "textFile/library.include.h"


namespace ObjectParser
{
	Lexer::Lexer(Compiler& compiler, boost::filesystem::path& file, std::ostream* out) :
		Lexer(compiler, TextFile::TextFromFile(file.string().c_str()).text(), out)
		
	{
		fileName = file.string();
	}

	Lexer::Lexer(Compiler& compiler, const std::string& text, std::ostream* out) :
		yyFlexLexer(&input, out),
		compiler(compiler),
		gc(compiler.result)
	{
		input.str(text);
	}

	Parser::token_type Lexer::operator()(Parser::semantic_type* lval, Parser::location_type* lloc)
	{
		yylval = lval;
		yylloc = lloc;
		return Parser::token_type(yylex());
	}


	void Lexer::LexerError(const char* msg)
	{
		LOG_ERROR("lexical error: " << msg);
	}

	void Lexer::printError(const char* msg, const Parser::location_type& yyloc)
	{
		LOG_ERROR("lexical error: at " << yyloc << " is '" << msg << "'");
	}

	std::string strlocation(const Parser::location_type& loc)
	{
		std::string fileName = *loc.begin.filename;
		position last = loc.end - 1;
		return str::stringize(boost::filesystem::canonical(fileName).string(), "(", last.line, "):").str();
	}
}