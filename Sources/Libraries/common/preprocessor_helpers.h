// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define MACRO_ID(x) x

#define STPP_STRINGIZE(something) STPP_STRINGIZE_HELPER(something) 
#define STPP_STRINGIZE_HELPER(something) #something
#define SOURCE_LOCATION __FILE__ "(" STPP_STRINGIZE(__LINE__) "):"
#define SOURCE_LOCATION_STR(exp) SOURCE_LOCATION "expression '" #exp "' failed'"
#define FUNCTION_LOCATION __FUNCTION__ "', line: " STPP_STRINGIZE(__LINE__)
#define COMPILER_WARNING(MESSAGE) "warning: " MESSAGE
#define COMPILER_ERROR(MESSAGE) "error: " MESSAGE 

#define STPP_STRINGIZE_EXPR(exp) str::spaced(STPP_STRINGIZE(exp), "=", (exp) ).str()
#define STPP_STRINGIZE_N1(exp) str::stringize(str::comma(), str::spaced(STPP_STRINGIZE(exp), "=", (exp))).str()

#define STPP_STRINGIZE_N2(exp0, exp1) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1) \
	).str()

#define STPP_STRINGIZE_N3(exp0, exp1, exp2) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2) \
	).str()

#define STPP_STRINGIZE_N4(exp0, exp1, exp2, exp3) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3) \
	).str()

#define STPP_STRINGIZE_N5(exp0, exp1, exp2, exp3, exp4) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3), \
	STPP_STRINGIZE_EXPR(exp4) \
	).str()

#define STPP_STRINGIZE_N6(exp0, exp1, exp2, exp3, exp4, exp5) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3), \
	STPP_STRINGIZE_EXPR(exp4), \
	STPP_STRINGIZE_EXPR(exp5) \
	).str()

#define STPP_STRINGIZE_N7(exp0, exp1, exp2, exp3, exp4, exp5, exp6) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3), \
	STPP_STRINGIZE_EXPR(exp4), \
	STPP_STRINGIZE_EXPR(exp5), \
	STPP_STRINGIZE_EXPR(exp6) \
	).str()

#define STPP_STRINGIZE_N8(exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3), \
	STPP_STRINGIZE_EXPR(exp4), \
	STPP_STRINGIZE_EXPR(exp5), \
	STPP_STRINGIZE_EXPR(exp6), \
	STPP_STRINGIZE_EXPR(exp7) \
	).str()

#define STPP_STRINGIZE_N9(exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3), \
	STPP_STRINGIZE_EXPR(exp4), \
	STPP_STRINGIZE_EXPR(exp5), \
	STPP_STRINGIZE_EXPR(exp6), \
	STPP_STRINGIZE_EXPR(exp7), \
	STPP_STRINGIZE_EXPR(exp8) \
	).str()

#define STPP_STRINGIZE_N10(exp0, exp1, exp2, exp3, exp4, exp5, exp6, exp7, exp8, exp9) str::stringize(str::comma(), \
	STPP_STRINGIZE_EXPR(exp0), \
	STPP_STRINGIZE_EXPR(exp1), \
	STPP_STRINGIZE_EXPR(exp2), \
	STPP_STRINGIZE_EXPR(exp3), \
	STPP_STRINGIZE_EXPR(exp4), \
	STPP_STRINGIZE_EXPR(exp5), \
	STPP_STRINGIZE_EXPR(exp6), \
	STPP_STRINGIZE_EXPR(exp7), \
	STPP_STRINGIZE_EXPR(exp8), \
	STPP_STRINGIZE_EXPR(exp9) \
	).str()

#define STPP_STRINGIZE_VALUES(...) MACRO_ID(GET_MACRO(__VA_ARGS__, \
	STPP_STRINGIZE_N10, \
	STPP_STRINGIZE_N9, \
	STPP_STRINGIZE_N8, \
	STPP_STRINGIZE_N7, \
	STPP_STRINGIZE_N6, \
	STPP_STRINGIZE_N5, \
	STPP_STRINGIZE_N4, \
	STPP_STRINGIZE_N3, \
	STPP_STRINGIZE_N2, \
	STPP_STRINGIZE_N1 \
	)(__VA_ARGS__))

#define EXVAL(expr) STPP_STRINGIZE_EXPR(expr)





// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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