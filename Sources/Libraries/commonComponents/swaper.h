#pragma once

#include "componentModel/library.include.h"

class Swaper : public SimpleComponent<Swaper>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Swaper);

	struct Resource : public ResourceBase 
	{
		void properties(Swaper& component);
		std::string className;
	};

	Swaper();

	void load(const Resource* res);
	void save(Resource* res) const;

	virtual void serialize(serializer& s);

	void swap();

private:
	const Class* class1;
	const Class* class2;
};
