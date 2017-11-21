#pragma once
#include "../color.h"
#include "../plane.h"
#include <map>


namespace image
{
	namespace filter
	{
		namespace mapping
		{
			template<typename SF, typename ST>
			struct Map : public std::map<SF, ST>
			{

			};

			//int to color
			struct RainbowGenerator
			{
				void setRange(const int first_, const int last_)
				{
					first = first_;
					last = last_;
				}

				template<Format fmt>
				Color<fmt> next(const int& param)
				{
					if (first == last)
					{
						//установка диапазона по умолчанию			
						first = 0;
						last = 35;
					}

					float h = (float)param / (float)(last+1) * 360;
					Color<fmt> color = hsv<fmt>(h, 1.0f, 1.0f);
					return color;
				}


				int first = 0;
				int last = 0;

			};

			template<Format fmt>
			Map<int, Color<fmt>> generate(const std::vector<int> set, RainbowGenerator& generator)
			{
				Map<int, Color<fmt>> result;

				ENFORCE(set.size() > 0);
				generator.setRange(set[0], set[set.size() - 1]);

				for (int iter : set)
				{
					result[iter] = generator.next<fmt>(iter);
				}

				return result;
			}


			static inline std::vector<int> range(int first, int last)
			{
				std::vector<int> result;
				int count = last - first;

				result.resize(count);

				for (int i = 0; i < count; i++)
				{
					result[i] = first+i;
				}

				return result;
			}



		}//
	}//
}//