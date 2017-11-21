#include "referencedComponents.h"

namespace ReferencesComponents
{
	//
	//
	//
	void ReferencedRed::report() {	LOG_REPORT("ReferencedRed"); }
	void ReferencedBlue::report(){	LOG_REPORT("ReferencedBlue");}

	void ReferencedRed::finalize()	{ /*LOG_MSG("ReferencedRed");*/ }
	void ReferencedBlue::finalize() { /*LOG_MSG("ReferencedBlue");*/ }
	//
	//
	//
	void ReferencedUtilizer::Resource::properties(ReferencedUtilizer& component)
	{
		property("references", component.links);
	}

	void ReferencedUtilizer::update()
	{
		LOG_REPORT("ReferencedUtilizer::update");
		links([](auto& link) 
		{
			link.report();
		});
	}	

	void ReferencedUtilizer::finalize() { /*LOG_MSG("ReferencedUtilizer");*/ }
}