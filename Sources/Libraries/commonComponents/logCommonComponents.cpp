#include "logCommonComponents.h"

namespace LogComponents
{
	void LogAccumulator::reset()
	{
		report.clear();
	}

	void LogAccumulator::append(const std::string& text)
	{
		report += text;
	}

	const std::string& LogAccumulator::result() const
	{
		return report;
	}

	std::string LogAccumulator::total;
	void LogAccumulator::update()
	{
		total += report;
		reset();
	}

	void PositionLogger::Resource::properties(PositionLogger& component)
	{
		property("verbose", component.verbose);
		property("accumulator", component.accumulator);
	}

	void PositionLogger::update()
	{
		auto& state = component<const Position>().state;

		str::spaced text("position:", state.position);
		if (verbose)
		{
			text("HPB:", state.GetHPB());
		}

		accumulator([&text](LogAccumulator& accum)
		{
			accum.append(text);
		});
	}
}