#pragma once

#include <boost/filesystem.hpp>

#include "../classDescEtc/grammarComposition.h"
#include "../unroll/unroller.h"
#include "../classDescEtc/utils/nameSource.h"
#include "stuff/atof.h"

#include "common/includes.h"
#include "expressions/library.include.h"

namespace ObjectParser
{
	//
	class ExtensionTable : std::unordered_map<std::string, std::unique_ptr<Expressions::Extension>>
	{
	public:
		virtual ~ExtensionTable(){}

		void addExtension(Expressions::Extension* ext) { operator[](ext->name()).reset(ext); }
		Expressions::Extension* extension(const std::string& name){ return operator[](name).get(); }
	};

	//
	class Compiler : public ExtensionTable
	{
	friend class Parser;
	public:
		Compiler();
		Compiler(const char* fileName);
		~Compiler();

		void compile(const boost::filesystem::path& fileName);
		GrammarComposition result;

	private:
		void loadClassDescInfo(const boost::filesystem::path& current);
		Includes includes;
	};
};