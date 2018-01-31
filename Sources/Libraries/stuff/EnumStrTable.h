// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//



#ifndef __EnumStrTable_H
#define __EnumStrTable_H


#define BEGIN_ENUM_STR_TABLE(_enum)\
	namespace enum_table_##_enum\
{\
struct\
	{\
	_enum e;\
	const char* str;\
	} table[]= {
#define END_ENUM_STR_TABLE(_enum)	};\
	unsigned int table_size = sizeof(table) / sizeof(table[0]);\
	const char* Find(_enum e)\
{\
	static const char* notFound = "";\
	for(unsigned int i = 0; i < table_size; i++) if (table[i].e == e) return table[i].str;\
	return notFound;\
}\
	_enum Find(const char* str)\
{\
	if (str) for(unsigned int i = 0; i < table_size; i++) if (::strcmp(table[i].str, str) == 0) return table[i].e;\
	return table[0].e;\
};}



#define DECLARE_ENUM_STR_TABLE(_enum)\
	namespace enum_table_##_enum\
{\
	const char* Find(_enum e);\
	_enum Find(const char* str);\
}


#define ENUM_STR(_e) { _e, #_e }


#define CONVERT_ENUM_TO_STR(_enum, e_value) enum_table_##_enum::Find(e_value)

#define CONVERT_STR_TO_ENUM(_enum, str) enum_table_##_enum::Find(str)

#endif 




// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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