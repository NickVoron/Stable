// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "jniComponents.h"
#include "commonAppModules/library.include.h"
#include "jni/library.include.h"

namespace JniComponents
{
	void JavaStaticMethodCaller::Resource::properties(JavaStaticMethodCaller& component)
	{
		property("classPath", component.classPath);
		property("functionName", component.functionName);
		property("functionSignature", component.functionSignature);
	}

	void JavaStaticMethodCaller::update()
	{
		call();
	}

	void JavaStaticMethodCaller::call()
	{
#ifdef ANDROID
        jni::CallGuard guard;
        auto method = jni::get_static_function(classPath, functionName, functionSignature);
        if(std::get<1>(method))
        {
            guard.env()->CallStaticVoidMethod(std::get<0>(method), std::get<1>(method));
        }
#endif
	}
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