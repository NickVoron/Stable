#include "compiler.h"

#include "expressions/library.include.h"
#include "reflection/library.include.h"

#include "../parser/lexer.h"
#include "../classDescEtc/classDesc.h"
#include "../classDescEtc/expressions/objectConverter.h"
#include "../classDescEtc/expressions/componentRefConverter.h"


bool convertLink(Expressions::EvaluationUnit& expr, mirror::Type<LinkDesc>& client) { return Expressions::convert(expr, client.val()); }
bool convertLinkList(Expressions::EvaluationUnit& expr, mirror::Type<LinksDescList>& client) { return Expressions::convert(expr, client.val()); }
bool convertValueAddress(Expressions::EvaluationUnit& expr, mirror::Type<ValueAddress>& client) { return Expressions::convert(expr, client.val()); }
bool convertPrototype(Expressions::EvaluationUnit& expr, mirror::Type<EntitiesStream>& client) { return Expressions::convert(expr, client.val()); }

struct ComponentModelInitializer
{
	ComponentModelInitializer()
	{	
		Expressions::FUNCTIONS::init();

		MIRROR_TYPE_DECLARE(LinkDesc);
		MIRROR_TYPE_DECLARE(LinksDescList);
		MIRROR_TYPE_DECLARE(ValueAddress);
		MIRROR_TYPE_DECLARE(EntitiesStream);
		MIRROR_STRUCT_DECLARE(Vector2, x, y);
		MIRROR_STRUCT_DECLARE(Vector3, x, y, z);
		MIRROR_STRUCT_DECLARE(Vector4, x, y, z, w);

		Expressions::multimethodsConversionTable.add(&convertLink);
		Expressions::multimethodsConversionTable.add(&convertLinkList);
		Expressions::multimethodsConversionTable.add(&convertValueAddress);
		Expressions::multimethodsConversionTable.add(&convertPrototype);
	}
};

namespace ObjectParser
{

Compiler::Compiler() : result(*this, *this)
{
	static ComponentModelInitializer helper;
	ComponentNameSource::reset();
}

Compiler::Compiler(const char* fileName):Compiler()						 
{
	compile(boost::filesystem::path(fileName));
}

Compiler::~Compiler()
{
	Expressions::collect_garbage();
}

void Compiler::compile(const boost::filesystem::path& fileName)
{
	LOG_MSG("compile script file: " << fileName.string());
	if (boost::filesystem::exists(fileName))
	{
		loadClassDescInfo(fileName.string().c_str());
	}	
}

void Compiler::loadClassDescInfo(const boost::filesystem::path& current)
{
	includes.add(current, [this](auto path)
	{
		Lexer lexer(*this, path);
		Parser(lexer).parse();
	});
}

}