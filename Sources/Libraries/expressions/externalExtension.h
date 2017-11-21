#pragma once
#include "expression.h"
#include "extension/extension.h"

namespace Expressions
{
	class ExternalExtension : public Expression
	{
	public:
		ExternalExtension(const std::string& extensionName, const std::string& text);
		
		virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const override;
	private:
		std::string extensionName;
		std::string text;
	};
}

