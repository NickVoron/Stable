#include "prototypeHandle.h"

namespace ObjectParser
{
	PrototypeHandle::PrototypeHandle(Expressions::EvaluatedScope& parentScope_, const InstanceDefinitionExpression& prototype_, ClassTable& classTable_, EntitiesList& entities_) :
		EvaluationUnit("prototype_handle", prototype, parentScope_),
		prototype(prototype_),
		classTable(classTable_),
		entities(entities_)
	{
	}

	Expressions::EvaluationInfo PrototypeHandle::evaluate()
	{
		Expressions::EvaluationInfo result(Expressions::Impossible);
		if(!object)
		{
			object = prototype.instance()->evaluated(scope());
			result.reject(this);
		}		
		else if(object->evaluate() == Expressions::Complete)
		{			
			result.complete(this);
		}
		return result;
	}

	void PrototypeHandle::extract(EvaluationSet& result)
	{
		if(object)
		{
			result.add(&object);
		}		
	}

}//