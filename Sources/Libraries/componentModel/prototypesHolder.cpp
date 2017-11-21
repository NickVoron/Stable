#include "prototypesHolder.h"

EntitiesLoadStream* PrototypeHolder::find(void* id) const
{
	auto it = data.find(id);
	return it != data.end() ? it->second.get() : 0;
}

void PrototypeHolder::clear()
{
	data.clear();
}

EntitiesLoadStream& PrototypeHolder::create(void* id, const std::string& className)
{
	auto& os = data[id];
	CM_KERNEL_ENFORCE(os == 0);
	os = std::make_unique<EntitiesLoadStream>();
	os->prototypeClassName = className;
	return *os;
}

void PrototypeHolder::save(stream::ostream& os) const
{
	os << data.size();

	for (auto& it : data)
	{
		os << it.first << it.second->prototypeClassName << *it.second;
	}
}

void PrototypeHolder::load(stream::istream& is)
{
	data.clear();

	std::size_t size = 0;
	is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		void* id;
		std::string className;
		is >> id >> className;
		auto& els = create(id, className);
		is >> els;
	}
}

std::string PrototypeHolder::debugstr() const
{
	str::stringize result("count: ", data.size(), "\n");

	for (auto& it : data)
	{
		result(str::delimeter("\n\t"), it.first, " : ", it.second->prototypeClassName);
	}

	return result.str();
}


std::ostream& operator<<(std::ostream& os, const EntitiesStream& es) { return os; }

stream::ostream& operator<<(stream::ostream& os, const EntitiesLoadStream& es)
{
	return os << es.ios;
}

stream::istream& operator>>(stream::istream& is, EntitiesLoadStream& es)
{
	return is >> es.ios;
}

stream::ostream& operator<<(stream::ostream& os, const ComponentExternalLink& link)
{
	return os << link.link.address;
}

stream::istream& operator>>(stream::istream& is, ComponentExternalLink& link)
{
	return is >> link.link.address;
}