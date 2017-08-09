// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "Vector3.h"

const Vector3 Vector3::nan		= Vector3(NAN, NAN, NAN);
const Vector3 Vector3::max		= Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
const Vector3 Vector3::zero		= Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one		= Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::xAxis	= Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::yAxis	= Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::zAxis	= Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::axes[3]  = {xAxis, yAxis, zAxis};
const Vector3 Vector3::xNegAxis = Vector3(-1.0f,  0.0f,  0.0f);
const Vector3 Vector3::yNegAxis = Vector3( 0.0f, -1.0f,  0.0f);
const Vector3 Vector3::zNegAxis = Vector3( 0.0f,  0.0f, -1.0f);





// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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