// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "image/library.include.h"

namespace unigui
{
	typedef image::Color<image::ARGB32F> Color;
	inline Color color(float r, float g, float b, float a)
	{
		return image::rgba<image::ARGB32F>(r, g, b, a);		
	}

	template<image::Format fmt>
	struct RandomColors : public std::vector<image::Color<fmt>>
	{
		RandomColors() {}
		RandomColors(int count) { init(count); }

		void init(int count)
		{
			this->resize(count);

			for (auto& clr : *this)
			{
				float r = Base::MathUtils::GetRandomThetha();
				float g = Base::MathUtils::GetRandomThetha();
				float b = Base::MathUtils::GetRandomThetha();
				float a = 1.0f;

				clr = image::rgba<fmt>(r, g, b, a);
			}
		}
	};
}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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