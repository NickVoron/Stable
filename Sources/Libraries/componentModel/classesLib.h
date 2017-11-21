#pragma once

#include "class.h"
#include "prototypesHolder.h"

class ClassesLib
{
public:
	~ClassesLib();
	void clear();
	void create(const ClassDescList& importedClasses, ExecutionList& executionList);
	const Class& find(const std::string& className) const;
	void save(stream::ostream& os) const;
	void load(stream::istream& is, ExecutionList& executionList);
	void merge(ClassesLib& other);
	void finalize(ExecutionList& executionList);

	void debugOutput() const;

	ClassMapping mapping;
	PrototypeHolder prototypes;
	ExecutionListFilter filter;
	std::vector<std::unique_ptr<Class>> classes;
};