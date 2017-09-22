// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expressions/library.include.h"
#include <iostream>
#include "stuff/userStruct.h"

namespace ComponentModelTesting
{
	
	struct SubObjectData
	{
		int index;
		std::string type;
	};

	struct SubObjectsList : public std::vector<SubObjectData> {};

	
	struct ObjectData
	{
		int index;
		std::string type;
		SubObjectsList subs;
	};

	struct ObjectsList: public std::vector<ObjectData>{};

	
	ObjectsList parseTree();

	
	int objectIndex(const ObjectData& objectData);
	std::string objectType(const ObjectData& objectData);

	int sunIndex(const SubObjectData& data);
	std::string subType(const SubObjectData& data);
	
	
	static const char* SUB_OBJECT_DATA = "SubObjectData";
	static const char* OBJECT_DATA = "ObjectData";

	
	
	std::ostream& operator<<(std::ostream& os, const SubObjectData& data);
	stream::ostream& operator<<(stream::ostream& os, const SubObjectData& data);
	stream::istream& operator >> (stream::istream& is, SubObjectData& data);

	
	
	
	inline std::ostream& operator<<(std::ostream& os, const SubObjectsList& list) { return UserStruct::streamOut(os, list); }
	inline stream::ostream& operator<<(stream::ostream& os, const SubObjectsList& list) { return UserStruct::stream_out(os, list); }
	inline stream::istream& operator >> (stream::istream& is, SubObjectsList& list) { return UserStruct::stream_in(is, list); }


	
	Expressions::Expression* convertType(const SubObjectData& value);
	Expressions::Expression* convertType(const SubObjectsList& value);

	
	
	
	std::ostream& operator<<(std::ostream& os, const ObjectData& data);
	stream::ostream& operator<<(stream::ostream& os, const ObjectData& data);
	stream::istream& operator >> (stream::istream& is, ObjectData& data);

	
	inline std::ostream& operator<<(std::ostream& os, const ObjectsList& list) { return UserStruct::streamOut(os, list); }
	inline stream::ostream& operator<<(stream::ostream& os, const ObjectsList& list) { return UserStruct::stream_out(os, list); }
	inline stream::istream& operator >> (stream::istream& is, ObjectsList& list) { return UserStruct::stream_in(is, list); }

	
	Expressions::Expression* convertType(const ObjectData& value);
}//



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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