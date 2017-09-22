// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "types.h"

namespace ComponentModelTesting
{
	ObjectsList parseTree()
	{
		ObjectsList result;

		for (int i=0; i<10; i++)
		{
			ObjectData object;
			object.index = i;
			object.type = str::stringize(i);
			
			for (int j=0; j<i; j++)
			{
				SubObjectData sub;
				sub.index = j;
				sub.type = str::stringize(j);
				object.subs.push_back(sub);
			}

			result.push_back(object);
		}

		return result;
	}

	int objectIndex(const ObjectData& objectData)
	{
		return objectData.index;
	}

	std::string objectType(const ObjectData& objectData)
	{
		return objectData.type;
	}

	int sunIndex(const SubObjectData& data)
	{
		return data.index;
	}

	std::string subType(const SubObjectData& data)
	{
		return data.type;
	}
	
	Expressions::Expression* convertType(const SubObjectData& value)
	{
		Expressions::EvalPropertiesStruct* data = Expressions::add<Expressions::EvalPropertiesStruct>("SubObjectData");
		data->add("index", Expressions::convertType(value.index)->cast<Expressions::EvaluationUnit>());
		data->add("type", Expressions::convertType(value.type)->cast<Expressions::EvaluationUnit>());

		return data;
	}


	
	
	std::ostream& operator<<(std::ostream& os, const SubObjectData& data)
	{
		os << data.index << data.type;
		return os;
	}

	stream::ostream& operator<<(stream::ostream& os, const SubObjectData& data)
	{
		os << data.index << data.type;
		return os;
	}

	stream::istream& operator >> (stream::istream& is, SubObjectData& data)
	{
		is >> data.index >> data.type;
		return is;
	}

	
	std::ostream& operator<<(std::ostream& os, const ObjectData& data)
	{
		return os << data.index << data.type << data.subs;
	}

	stream::ostream& operator<<(stream::ostream& os, const ObjectData& data)
	{
		return os << data.index << data.type << data.subs;
	}

	stream::istream& operator >> (stream::istream& is, ObjectData& data)
	{
		return is >> data.index >> data.type >> data.subs;
	}

	
	Expressions::Expression* convertType(const ObjectData& value)
	{
		Expressions::EvalPropertiesStruct* data = Expressions::add<Expressions::EvalPropertiesStruct>("ObjectData");
		 
		data->add("index", Expressions::convertType(value.index)->cast<Expressions::EvaluationUnit>());
		data->add("type", Expressions::convertType(value.type)->cast<Expressions::EvaluationUnit>());
		data->add("subs", Expressions::convertType(value.subs)->cast<Expressions::EvaluationUnit>());
		return data;
	}

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