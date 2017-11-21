#include "expression.h"
#include "namescope.h"
#include "descParser/library.include.h"

namespace Expressions
{
	EvaluatedScope EvaluationUnit::commonParent("commonParent", nullptr);

	EvaluationState merge(const EvaluationState first, const EvaluationState second)
	{
		if (first == Complete && second == Complete)		return Complete;
		if (first == Complete && second == Reject)			return Reject;
		if (first == Complete && second == Impossible)		return Impossible;
		if (first == Complete && second == Empty)			return Complete;

		if (first == Reject && second == Complete)			return Reject;
		if (first == Reject && second == Reject)			return Reject;
		if (first == Reject && second == Impossible)		return Reject;
		if (first == Reject && second == Empty)				return Reject;

		if (first == Impossible && second == Complete)		return Impossible;
		if (first == Impossible && second == Reject)		return Reject;
		if (first == Impossible && second == Impossible)	return Impossible;
		if (first == Impossible && second == Empty)			return Impossible;

		if (first == Empty && second == Complete)			return Complete;
		if (first == Empty && second == Reject)				return Reject;
		if (first == Empty && second == Impossible)			return Impossible;
		if (first == Empty && second == Empty)				return Empty;

		ENFORCE(false);
		return Impossible;
	}

	void EvaluationInfo::operator+=(const EvaluationInfo& info)
	{
		replace(nullptr);
		evalstate = merge(evalstate, info.evalstate);
	}

	EvaluationInfo EvaluationInfo::operator+(const EvaluationInfo& info) const
	{ 
		EvaluationInfo res = *this; 
		res += info; 
		return res; 
	}


	void EvaluationInfo::reject(EvaluationUnit* evaluated)
	{
		replace(evaluated);
		evalstate = Reject;
	}

	void EvaluationInfo::impossible() 
	{ 
		replace(nullptr);
		evalstate = Impossible; 
	}

	void EvaluationInfo::complete(EvaluationUnit* evaluated)
	{
		replace(evaluated);
		evalstate = Complete;
	}

	EvaluationInfo iterateEvaluation(EvaluationUnit*& unit)
	{
		ENFORCE_POINTER(unit);
		auto result = unit->evaluate();
		if (auto replacement = result.replacement())
		{
			unit = replacement;
		}
		return result;
	}

	//
	std::ostream& operator<<(std::ostream& os, const EvaluatedScope& ctx)
	{
		return os;
	}

	//
	//
	//
	std::string Expression::location_str() const
	{
		std::string result;
		if (!loc.filename.empty())
		{
			result = str::msvs_out(loc.filename, loc.line, string());
		}
		return result;
	}

	//
	//
	//
	//Expression
	EvaluationUnit::EvaluationUnit(const std::string& namescopeName, const Expression& protoexpr_, EvaluatedScope& parent_) :
			protoexpr(protoexpr_)
		,	internalScope(namescopeName, &parent_)
	{

	}
	 
	EvaluationInfo EvaluationUnit::evaluate()
	{
		EvaluationInfo evalinfo;
		evalinfo.complete(this);
		return evalinfo;
	}

	std::string EvaluationUnit::path() const
	{
		return str::stringize(internalScope.path(), ": (", string(), ")");
	}


	bool EvaluationUnit::EvaluationSet::exists(EvaluationUnit** unit) const
	{
		return result.find(unit) != result.end();
	}

	void EvaluationUnit::EvaluationSet::add(EvaluationUnit** unit)
	{
		result.insert(unit);
	}

	void extractDependencies(EvaluatedScope& scope, EvaluationUnit::EvaluationSet& result)
	{
		for (auto& value : scope)
		{
			if (!result.exists(&value.second))
			{
				result.add(&value.second);
				value.second->extract(result);
			}
		}
	}

	void EvaluationUnit::extract(EvaluationSet& result)
	{
		extractDependencies(internalScope, result);
	}

	bool compare(const std::vector<EvaluationUnit**>& lhs, const std::vector<EvaluationUnit**>& rhs)
	{
		std::vector<EvaluationUnit*> vlhs(lhs.size());
		std::vector<EvaluationUnit*> vrhs(rhs.size());
		std::transform(lhs.begin(), lhs.end(), vlhs.begin(), [](auto p) { return *p; });
		std::transform(rhs.begin(), rhs.end(), vrhs.begin(), [](auto p) { return *p; });

		return vlhs == vrhs;
	}

	std::vector<EvaluationUnit**> dependencies(Expressions::EvaluationUnit*& unit)
	{
		EvaluationUnit::EvaluationSet units;
		units.add(&unit);
		unit->extract(units);
		return { units.result.begin(), units.result.end() };
		/*
		auto pred = [](EvaluationUnit** left, EvaluationUnit** right)
		{
			if (left && right)
			{
				return (*left)->protoexpr.loc.line < (*right)->protoexpr.loc.line;
			}
			return left < right;
		};
		std::vector<EvaluationUnit**> sorted(units.begin(), units.end());
		std::sort(sorted.begin(), sorted.end(), pred);
		return sorted;
		*/
	}

	template<class Fn>
	auto filter_by_result(const std::vector<EvaluationUnit**> input, Fn&& function)
	{
		std::vector<EvaluationUnit**> result;
		std::for_each(input.begin(), input.end(), [&result, &function](auto inparam)
		{
			auto& unitref = (*inparam);
			if(function(unitref->evaluate()))
			{
				result.push_back(inparam);
			}
		});

		return result;
	}

	auto filter_by_state(const std::vector<EvaluationUnit**> input, EvaluationState state)
	{
		return filter_by_result(input, [state](auto& evalinfo)
		{
			return evalinfo != state;
		});
	}

	auto filter_incompleted(const std::vector<EvaluationUnit**> input)
	{
		return filter_by_state(input, Complete);
	}

	//
	//
	//
	template<class EvaluationUnitType>
	auto filter_by_type(const std::vector<EvaluationUnit**> input, bool invert)
	{
		std::vector<EvaluationUnit**> result;

		std::for_each(input.begin(), input.end(), [&result, invert](auto inparam)
		{
			auto& unitref = (*inparam);
			if (!invert && unitref->cast<EvaluationUnitType>())
			{
				result.push_back(inparam);
			}
			else if(invert && !unitref->cast<EvaluationUnitType>())
			{
				result.push_back(inparam);
			}
		});

		return result;
	}


	bool uneval_report = false;
	std::set<unsigned int> unevaluated_breakpoints;

	auto iterate_once(std::vector<EvaluationUnit**>& input, std::vector<EvaluationUnit**>& incompleted)
	{
		EvaluationInfo state;
		for (auto& unitptr : input)
		{
			auto& unitref = (*unitptr);
			auto res = iterateEvaluation(unitref);
			if(res != Complete)
			{
				incompleted.push_back(unitptr);
			}

			state += res;
		}

		return state;
	}

	void debug_iterate_once(std::vector<EvaluationUnit**>& input, const std::set<unsigned int>& breakpoints)
	{
		for (auto& unitptr : input)
		{
			auto& unitref = (*unitptr);
			auto state = iterateEvaluation(unitref);
			if(state != Complete)
			{
				auto line = unitref->protoexpr.loc.line;
				if(breakpoints.count(line))
				{
					state = iterateEvaluation(unitref);
				}
			}
		}
	}

	void uneval_break(unsigned int line)
	{
		unevaluated_breakpoints.insert(line);
	}

	void report_uneval(bool enabled)
	{
		uneval_report = enabled;
	}

	void print_scope(EvaluatedScope& namescope, int tabs)
	{
		//LOG_MSG(logs::tabs(tabs) << namescope.path());
		LOG_MSG(logs::tabs(tabs) << namescope.string());
		for(auto& unit : namescope)
		{
			LOG_MSG(logs::tabs(tabs + 1) << unit.first);
			print_scope(unit.second->scope(), tabs + 1);
		}
	}

	void print_scope_from_root(EvaluatedScope& namescope)
	{
		EvaluatedScope* root = &namescope;
		while (root->parent) 
		{
			print_scope(*root, 0);
			root = root->parent;
		}	
		
		print_scope(*root, 0);
	}

	EvaluationUnit* unrollExpression(const Expression& expression, EvaluatedScope& namescope, bool debugLog)
	{
		auto unit = expression.evaluated(namescope);
		EvaluationInfo state;
		do
		{
			auto units = dependencies(unit);
			decltype(units) incompleted;
			if (!units.empty())
			{
				state = iterate_once(units, incompleted);
			}
		} while (state != Complete);

		return unit;
	}

/*
	if(!incompleted.empty() && (oldincompleted == incompleted))
	{
	decltype(units) lastresult;
	iterate_once(incompleted, lastresult);
	if(!lastresult.empty())
	{
	for (auto& unitptr : lastresult)
	{
	auto& unitref = (*unitptr);
	LOG_MSG_UNDECORATED(unitref->protoexpr.location_str() << " " << unitref->path());
	}
	break;
	}
	}

	oldincompleted = incompleted;
	auto newincomplete = filter_incompleted(units);

	if(!newincomplete.empty())
	{
	auto references = filter_by_type<ReferenceUnit>(newincomplete, false);
	auto nonreferences = filter_by_type<ReferenceUnit>(newincomplete, true);
	ENFORCE_EQUAL(newincomplete.size(), references.size() + nonreferences.size());
	iterate_once(references);
	auto nunits = dependencies(unit);

	if(uneval_report)
	{
	LOG_MSG("===============================================");
	for (auto& unitptr : nonreferences)
	{
	auto& unitref = (*unitptr);
	LOG_MSG(unitref->path());
	auto deps = dependencies(unitref);
	for(auto dependence : deps)
	{
	//LOG_MSG_UNDECORATED((*dependence)->protoexpr.location_str() << " " << (*dependence)->path());
	}
	}
	}
	}*/
}
