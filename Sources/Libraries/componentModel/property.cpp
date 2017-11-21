#include "property.h"
#include "factory.h"
#include "prototypesHolder.h"

void ResourceTable::dump(ResourceDriver::ResourceBase& res) const 
{
	for(auto& v : *this)
	{
//		v.second->dump(v.first.c_str(), res);
	}
}

void ResourceTable::edit(ResourceDriver::ResourceBase& res) const
{
	for (auto& v : *this)
	{
//		v.second->edit(v.first.c_str(), res);
	}
}

const Property* ResourceTable::findProperty(const str::string32& name) const
{
	auto it = find(name);
	return it != end() ? it->second.get() : nullptr;
}

//
//
//
void Property::evaluateExternalLink(ComponentExternalLink& externalLink, ComponentBase& component, const Expressions::EvaluationUnit& rhs) const
{
	LinkDesc link;
	Expressions::convertVar(rhs, link);
	auto& checklink = externalLink.link;

	ENFORCE(link.address.objectIndex == checklink.address.objectIndex);
	ENFORCE(link.address.componentIndices == checklink.address.componentIndices);
}