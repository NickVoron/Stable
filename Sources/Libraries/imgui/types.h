// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <map>

#include "unigui/library.include.h"
#include "tweener/library.include.h"

namespace imgui
{

	typedef tween::Tweener<float, 1> VerticalScrollTweener;

	typedef unigui::math::Point2<float> Point2;
	typedef unigui::math::Rect<float> Rect;
	typedef unigui::Color Color;

	bool underMouse(const Rect& rect);

	struct Id
	{
		Id(){}
		Id(const Point2& p, int t):pos(p), type(t){}

		Point2 pos;
		int type = 0;
		bool empty = true;

		bool operator==(const Id& i) const { return pos == i.pos && type == i.type; }
		bool operator<(const Id& i) const { return pos.x < i.pos.x || (pos.x == i.pos.x && pos.y < i.pos.y) || (pos.x == i.pos.x && pos.y == i.pos.y && type < i.type); }
	};


	std::ostream& operator<<(std::ostream& os, const Id& id);

	struct Mouse
	{
		enum ButtonState
		{
			UP,
			DOWN,
			NOW_UP,
			NOW_DOWN,
		};
														 		
		void addProcessed(bool processed);
		bool processed() const;
		
		Point2 pos;
		float wheelDelta = 0.0f;
		ButtonState lmb = UP;
		bool processedFlag = false;
	};

	enum ButtonState { NORMAL, UNDER_MOUSE, HOT, BUTTON_STATES_COUNT };

	struct Keyboard
	{
	};


	template<class ControlState>
	struct ControlStateCache
	{
		typedef std::map<Id, ControlState> Data;

		bool stateIsEqualTo(const Id& controlId, const ControlState& state) const
		{
			auto it = data.find(controlId);
			return (it != data.end()) ? it->second == state : false;
		}

		void setState(const Id& controlId, const ControlState& state)
		{
			data[controlId] = state;
		}

		ControlState& state(const Id& controlId)
		{
			return data[controlId];
		}

		Data data;
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