#pragma once
#include "expressions/library.include.h"


namespace ObjectParser
{
	class ConstantProperty : public Expressions::Expression
	{
	public:
		ConstantProperty(const std::string& name, const Expressions::Expression* value);

		virtual std::string string() const override;

		void reset();
		void calculate() const;

		const std::string name;

	private:
		const Expressions::Expression* value;
	};
}//