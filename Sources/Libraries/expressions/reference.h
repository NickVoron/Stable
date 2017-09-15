// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expression.h"


namespace Expressions
{


class This : public EvaluationUnit
{
public:
	virtual bool boolean() const { return true; }
	virtual std::string string() const { return "this"; }
};


class Reference : public Expression
{
public:
	struct PathElement
	{
		virtual ~PathElement() {}

		virtual EvaluationUnit* evaluate(const EvaluationUnit* input, const EvaluatedScope& context) const = 0;
		virtual std::unique_ptr<PathElement> copy() const = 0;
		virtual std::string string() const { return ""; }
	};
		
	
	struct Path : std::vector<std::unique_ptr<PathElement>>
	{
		EvaluationUnit* evaluate(const EvaluatedScope& environment, boost::any* userData = 0) const;
		bool canResolveReverence(const EvaluatedScope& rootEnvironment) const;
		std::string string() const;

		std::string root() const; 
	};

	
	Reference() {}
	Reference(PathElement* element);

	void addPathElement(PathElement* element);

	const Path& getPath() const { return path; } 
	virtual std::string string() const override;
	virtual EvaluationUnit* evaluated(const EvaluatedScope& environment, boost::any* userData = 0) const override;

	virtual References references() const override;
	bool canResolveReverence(const EvaluatedScope& rootEnvironment) const;

	bool extraFlag;

protected:
	Path path;
};


struct PropertyPath : public Expressions::Reference::PathElement
{
	PropertyPath(const std::string& name_) : name(name_){}
	virtual EvaluationUnit* evaluate(const EvaluationUnit* previous, const EvaluatedScope& context) const override;
	virtual std::unique_ptr<PathElement> copy() const override;
	virtual std::string string() const override { return name; }

	const std::string name;

};



struct ArrayPath : public Reference::PathElement
{
	ArrayPath(int index_) : index(index_){}
	const size_t index;

	virtual EvaluationUnit* evaluate(const EvaluationUnit* input, const EvaluatedScope& context) const override;
	virtual std::unique_ptr<PathElement> copy() const override;
	virtual std::string string() const override { return "["+std::to_string(index)+"]"; }
};



}




// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>
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