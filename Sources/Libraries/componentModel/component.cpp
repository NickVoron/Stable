#include "component.h"
#include "class.h"

FinalizeQuery& LinkerInterface::query() { return ComponentsFactory::constructQuery(classIndex()); }