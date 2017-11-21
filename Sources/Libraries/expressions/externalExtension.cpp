#include "externalExtension.h"



namespace Expressions
{

ExternalExtension::ExternalExtension( const std::string& extensionName_, const std::string& text_) :
	extensionName(extensionName_), 
	text(text_)
{
}

EvaluationUnit* ExternalExtension::evaluated(EvaluatedScope& namescope) const
{
	return add<Const<std::string>>(*this, text);
}

}//
