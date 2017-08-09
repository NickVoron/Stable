// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.