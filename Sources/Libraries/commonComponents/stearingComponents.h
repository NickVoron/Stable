// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "commonComponents/library.include.h"

namespace StearingComponents
{

class StearingTargetsHolder;
class StearingTarget : public CustomComponent<StearingTarget, StearingTargetsHolder>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(StearingTarget);

	void linker();

	Position* position;
};

class StearingTargetsHolder : public SimpleComponentInputParameters<StearingTarget, void>
{
public:
	StearingTarget* findNearest(const Vector3& position);
};

class NearestTargetSearcher : public UpdatableComponent<NearestTargetSearcher>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(NearestTargetSearcher);

	NearestTargetSearcher();
	void update(float dt);
	void linker();

	Position* position;

	ComponentLink<StearingTarget> target;
};

class SeekStearing : public UpdatableComponent<SeekStearing>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(SeekStearing);

	SeekStearing();
	void update(float dt);
	void linker();						  

	Position* position;
	LinearMover* mover;
	NearestTargetSearcher* searcher;
};

typedef ComponentModel::Module<SeekStearing, StearingTarget, NearestTargetSearcher> Module;

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