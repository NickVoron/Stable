#pragma once

#include "componentModel/library.include.h"

namespace ReferencesComponents
{

class ReferencedBase : public SimpleComponent<ReferencedBase>
{
public:
	CM_DEFINE_BASE_CLSID(ReferencedBase)

	virtual void report() = 0;
};


//
//
//
class ReferencedRed : public ReferencedBase
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ReferencedRed);
	virtual void report() override;
	virtual void finalize() override;
};

class ReferencedBlue : public ReferencedBase
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ReferencedBlue);
	virtual void report() override;
	virtual void finalize() override;
};

//
//
//
class ReferencedUtilizer : public UpdatableComponent<ReferencedUtilizer>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ReferencedUtilizer);

	struct Resource : public ResourceBase 
	{
		void properties(ReferencedUtilizer& component);
	};
													
	void update();
	virtual void finalize() override;
	
	ComponentLinkList<ReferencedBase> links;
};

typedef ComponentModel::Module<ReferencedUtilizer, ReferencedBase, ReferencedRed, ReferencedBlue> Module;

}


