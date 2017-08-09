// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
}



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