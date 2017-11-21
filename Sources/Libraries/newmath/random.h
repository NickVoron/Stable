#pragma once

namespace nm
{
	template<class T> T random();
	template<class T> T random(T v1, T v2);
	
	float random_urnd();
	float random_srnd();
}