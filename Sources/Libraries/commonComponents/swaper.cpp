#include "swaper.h"

void Swaper::Resource::properties(Swaper& component)
{
//	add("className", className);
}

Swaper::Swaper():class1(0), class2(0)
{

}

void Swaper::load(const Resource* res)
{
	//class2 = &parent->getParent().classes.find(res->className);
	ENFORCE(class2);
}

void Swaper::save(Resource* res) const
{
	res->className = class2->name();
}

void Swaper::serialize(serializer& s)
{
	//INCOMPLETE;
}


void Swaper::swap()
{
	//CM_KERNEL_ENFORCE(parent && class2);
	//parent->changeClass(*class2);
}