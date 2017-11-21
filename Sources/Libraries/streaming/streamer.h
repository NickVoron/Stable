#pragma once
#include "queue.h"

namespace Streaming
{
	class Streamer
	{
	public:
		template<class QueryType>
		static void add(const typename QueryType::Params& params)
		{
			add(new QueryType(params));
		}

		static void add(LoaderBase* loader);

		static void commit();
		static bool active();
		static void init();
		static void release();
	};
}
