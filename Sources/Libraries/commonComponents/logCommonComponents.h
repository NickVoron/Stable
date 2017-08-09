// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "commonComponents/library.include.h"

namespace LogComponents
{														
	class PositionLogger : public UpdatableComponent<PositionLogger>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(PositionLogger);

		void update(float dt);
		void linker();

	private:
		const Position* position;
	};

	class LinearMoverBreaker : public UpdatableComponent<LinearMoverBreaker>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(LinearMoverBreaker);

		void update(float dt);
		void linker();

	private:
		LinearMover* mover;
	};

typedef ComponentsCore::Module<PositionLogger, LinearMoverBreaker> Module;

}






// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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