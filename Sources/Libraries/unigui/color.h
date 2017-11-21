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
