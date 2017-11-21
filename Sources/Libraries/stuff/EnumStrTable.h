/*
Copyright (c) 2006 Michael Pizik / Home work
Date: 29.5.2006 21:29
Description:	макросы для создания таблиц и функций конвертации enum <-> str.s
Пример использования:

---------------h файл:
namespace GUI
{
	enum HORIZONTAL_FORMATTING{HF_CENTRE_ALIGNED, HF_LEFT_ALIGNED};
	// описывать в h файле
	DECLARE_ENUM_STR_TABLE(HORIZONTAL_FORMATTING);
...
---------------cpp файл:
namespace GUI		// <------!!!!! заключать в те же неймспейсы что и enum, иначе не проходит, или объявлять таблицу сразу в h файле
{
// описывать таблицу в cpp файле
BEGIN_ENUM_STR_TABLE(HORIZONTAL_FORMATTING)
{HF_CENTRE_ALIGNED, "CenterAligned"},
{HF_LEFT_ALIGNED,	"LeftAligned"},
END_ENUM_STR_TABLE(HORIZONTAL_FORMATTING);
}

// конвертация
HORIZONTAL_FORMATTING hf = CONVERT_STR_TO_ENUM(HORIZONTAL_FORMATTING, "bla bla bla");
const char* s = CONVERT_ENUM_TO_STR(HORIZONTAL_FORMATTING, HF_CENTRE_ALIGNED);
...
*/

#ifndef __EnumStrTable_H
#define __EnumStrTable_H

// таблица конвертации для cpp файла
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


// декларация для h файла
#define DECLARE_ENUM_STR_TABLE(_enum)\
	namespace enum_table_##_enum\
{\
	const char* Find(_enum e);\
	_enum Find(const char* str);\
}

// конвертация прямая Enum в строку
#define ENUM_STR(_e) { _e, #_e }

// конвертация в строку, в случае ошибки вернёт 0
#define CONVERT_ENUM_TO_STR(_enum, e_value) enum_table_##_enum::Find(e_value)
// конвертация в enum, в случае ошибки вернёт первое вхождение в таблицу
#define CONVERT_STR_TO_ENUM(_enum, str) enum_table_##_enum::Find(str)

#endif //__EnumStrTable_H
