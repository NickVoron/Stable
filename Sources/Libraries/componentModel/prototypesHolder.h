#pragma once

#include <string>
#include <map>
#include <memory>

#include "links.h"

struct EntitiesStream
{
	void* streamId = nullptr;
};

struct EntitiesLoadStream
{
	std::string prototypeClassName;
	stream::dmemstream ios;
};

struct ComponentExternalLink
{
	LinkDesc link;
};

stream::ostream& operator<<(stream::ostream& os, const EntitiesLoadStream& es);
stream::istream& operator>>(stream::istream& is, EntitiesLoadStream& es);

stream::ostream& operator<<(stream::ostream& os, const ComponentExternalLink& link);
stream::istream& operator>>(stream::istream& is, ComponentExternalLink& link);

struct PrototypeHolder
{
public:
	void clear();
	EntitiesLoadStream* find(void* id) const;
	EntitiesLoadStream& create(void* id, const std::string& className);
	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	std::string debugstr() const;

private:
	std::map<void*, std::unique_ptr<EntitiesLoadStream>> data;
};

std::ostream& operator<<(std::ostream& os, const EntitiesStream& es);