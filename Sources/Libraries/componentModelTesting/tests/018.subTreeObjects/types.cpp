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
	//SubObjectData c++ tp script
	Expressions::EvaluationUnit* convertType(const SubObjectData& value)
	{
		auto proto = Expressions::add<Expressions::PropertiesStruct>("SubObjectData");
		auto data = proto->evaluated(Expressions::EvaluationUnit::commonParent)->cast<Expressions::PropertiesStructUnit>();
		data->scope().add("index", Expressions::convertType(value.index), Expressions::InsertMethod::INSERT);
		data->scope().add("type", Expressions::convertType(value.type), Expressions::InsertMethod::INSERT);
		return data;
	}


	// SubObjectData
	//stream
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

	//ObjectsList
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

	//ObjectData c++ tp script
	Expressions::EvaluationUnit* convertType(const ObjectData& value)
	{
		auto proto = Expressions::add<Expressions::PropertiesStruct>("ObjectData");
		auto data = proto->evaluated(Expressions::EvaluationUnit::commonParent)->cast<Expressions::PropertiesStructUnit>();
		data->scope().add("index", Expressions::convertType(value.index), Expressions::InsertMethod::INSERT);
		data->scope().add("type", Expressions::convertType(value.type), Expressions::InsertMethod::INSERT);
		data->scope().add("subs", Expressions::convertType(value.subs), Expressions::InsertMethod::INSERT);
		return data;
	}

}//