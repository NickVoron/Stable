#pragma once

#include "finalizeQuery.h"

struct ComponentDesc;
struct ClassDesc;
struct ClassDescList;

struct ClassDesc : public Base::NonCopyable
{
	struct LocalObject
	{
		std::string objectName;
		std::string className;
		std::size_t classId;
	};

	struct LocalObjects : std::vector<LocalObject>
	{
	};

	struct ComponentsList : public std::vector<ComponentDesc*>
	{
		~ComponentsList();
		int index(const std::string& name) const;
	};

	ClassDesc() {}
	ClassDesc(const std::string& clsName, std::size_t classIdx);
	~ClassDesc();

	void clear();
	ComponentDesc* addComponent(const std::string& className, const std::string& name, const std::string& category);
	void finalize();

	void categoryClass(const std::string& categoryName, ClassDesc& desc) const;
	void debug() const;

	std::size_t classIndex = -1;
	std::string className;
	ComponentsList components;
	LocalObjects localObjects;	
	std::set<std::string> categories;
private:
	void reorder();
	void unrollGraph(std::list<ComponentDesc*>& graph, ComponentDesc* cd);
	void spliceLinks(const ComponentDesc* srcComponent, ComponentDesc* dstComponent, ClassDesc& desc) const;
};

struct ComponentDesc : public Base::NonCopyable
{
	ComponentDesc(const std::string& className, const std::string& name, const std::string& category, const ClassDesc& parent);

	std::pair<std::size_t, bool> linkIndex(const std::string& clsId, const std::string& alias) const;
	void debug() const;
	
	const std::string className;
	const std::string name;
	const std::string category;
	const ClassDesc& parent;
	
	std::vector<ComponentDesc*> dependencies;
	FinalizeQuery& query;
};

struct ClassDescList : public std::vector<std::unique_ptr<ClassDesc>>
{
	ClassDesc& add(const std::string& className);
	std::pair<std::size_t, bool> findIndex(const std::string& className) const;
	void categorySlice(const std::string& category, ClassDescList& slice) const;
	void finalize();
	void debug() const;
};