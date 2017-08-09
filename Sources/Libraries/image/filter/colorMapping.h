// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



		}
	}
}//



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.