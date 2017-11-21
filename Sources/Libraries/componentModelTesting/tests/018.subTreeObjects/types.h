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

	//
	struct ObjectData
	{
		int index;
		std::string type;
		SubObjectsList subs;
	};

	struct ObjectsList: public std::vector<ObjectData>{};

	//
	ObjectsList parseTree();

	//
	int objectIndex(const ObjectData& objectData);
	std::string objectType(const ObjectData& objectData);

	int sunIndex(const SubObjectData& data);
	std::string subType(const SubObjectData& data);
	
	//converters
	static const char* SUB_OBJECT_DATA = "SubObjectData";
	static const char* OBJECT_DATA = "ObjectData";

	// SubObjectData
	//stream
	std::ostream& operator<<(std::ostream& os, const SubObjectData& data);
	stream::ostream& operator<<(stream::ostream& os, const SubObjectData& data);
	stream::istream& operator >> (stream::istream& is, SubObjectData& data);

	//
	//SubObjectsList
	//stream
	inline std::ostream& operator<<(std::ostream& os, const SubObjectsList& list) { return UserStruct::streamOut(os, list); }
	inline stream::ostream& operator<<(stream::ostream& os, const SubObjectsList& list) { return UserStruct::stream_out(os, list); }
	inline stream::istream& operator >> (stream::istream& is, SubObjectsList& list) { return UserStruct::stream_in(is, list); }


	//c++ tp script
	Expressions::EvaluationUnit* convertType(const SubObjectData& value);
	Expressions::EvaluationUnit* convertType(const SubObjectsList& value);
	Expressions::EvaluationUnit* convertType(const ObjectData& value);

	//ObjectData
	//stream
	//stream
	std::ostream& operator<<(std::ostream& os, const ObjectData& data);
	stream::ostream& operator<<(stream::ostream& os, const ObjectData& data);
	stream::istream& operator >> (stream::istream& is, ObjectData& data);

	//ObjectsList
	inline std::ostream& operator<<(std::ostream& os, const ObjectsList& list) { return UserStruct::streamOut(os, list); }
	inline stream::ostream& operator<<(stream::ostream& os, const ObjectsList& list) { return UserStruct::stream_out(os, list); }
	inline stream::istream& operator >> (stream::istream& is, ObjectsList& list) { return UserStruct::stream_in(is, list); }

}//