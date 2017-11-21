#pragma once

#include "position.h"
#include "linearMover.h"

namespace LogComponents
{	
	class LogAccumulator : public UpdatableComponentAutoLink<LogAccumulator>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(LogAccumulator);

		void reset();
		void append(const std::string& text);
		const std::string& result() const;

		void update();
		static std::string total;
	private:
		std::string report;
	};

	class PositionLogger : public UpdatableComponentAutoLink<PositionLogger, const Position>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(PositionLogger);

		struct Resource : public ResourceBase
		{
			void properties(PositionLogger& component);
		};

		void update();

		ComponentLink<LogAccumulator> accumulator;
		bool verbose = false;
	};

typedef ComponentModel::Module<LogAccumulator, PositionLogger> Module;

}


