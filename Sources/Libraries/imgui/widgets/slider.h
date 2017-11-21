#pragma once
#include "../types.h"

namespace imgui
{
// 	struct SliderDesc
// 	{
// 		SliderDesc(const std::string& caption, const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal);
// 		SliderDesc(const std::string& caption, const Vector3& pos, int inputFieldLength, int length, float minVal, float maxVal);
// 		SliderDesc(const std::string& caption, int inputFieldLength, int length, float minVal, float maxVal);
// 		SliderDesc(const std::string& caption, int length, float minVal, float maxVal);
// 
// 		SliderDesc(const std::string& caption, const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal);
// 		SliderDesc(const std::string& caption, const Vector3& pos, int inputFieldLength, int length, float minVal, float maxVal);
// 		SliderDesc(const std::string& caption, int inputFieldLength, int length, float minVal, float maxVal);
// 		SliderDesc(const std::string& caption, int length, float minVal, float maxVal);
// 
// 
// 		std::string caption;
// 		Point2 screenPosition;
// 		int inputFieldLength;
// 		int length;
// 		Base::Interval<float> interval;
// 	};

	float slider(const char* caption, const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal, float& val);
	float slider(const char* caption, const Point2& pos, int length, float minVal, float maxVal, float& val);
	float slider(const char* caption, int inputFieldLength, int length, float minVal, float maxVal, float& val);
	float slider(const char* caption, int length, float minVal, float maxVal, float& val);
	float slider(const char* caption, float minVal, float maxVal, float& val);

	float slider_angle(const char* caption, float& val);

	float slider(const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal, float& val);
	float slider(const Point2& pos, int length, float minVal, float maxVal, float& val);
	float slider(int inputFieldLength, int length, float minVal, float maxVal, float& val);
	float slider(int length, float minVal, float maxVal, float& val);
	float slider(float minVal, float maxVal, float& val);

	float slider(const char* caption, const Base::Interval<float>& interval, float& val);
	float slider(const char* caption, const Base::Interval<float>& interval, float& val);

	float slider(const char* caption, const Vector3& pos, float minVal, float maxVal, float& val);
	float slider(const Vector3& pos, float minVal, float maxVal, float& val);

	template<class PropertyGET, class PropertySET>
	float slider(float minVal, float maxVal, const PropertyGET& get, const PropertySET& set)
	{
		float val = get();
		imgui::slider(minVal, maxVal, val);
		set(val);
		return val;
	}

}
