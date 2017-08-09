// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#define COMPONENT_MODEL_KERNEL_DEBUG
#define COMPONENT_MODEL_REPORT_STATS

#ifdef COMPONENT_MODEL_KERNEL_DEBUG
#include "stuff/library.include.h"
#define CM_KERNEL_ENFORCE(exp) ENFORCE(exp)
#define CM_KERNEL_ENFORCE_MSG(exp, msg) ENFORCE_MSG(exp, msg)
#else
#define CM_KERNEL_ENFORCE(exp) (void(0))
#define CM_KERNEL_ENFORCE_MSG(exp, msg) (void(0))
#endif

#define CM_KERNEL_THROW(exp) do { throw Base::Errors::Simple(str::stringize(exp).str()); } while (0);







// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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