// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#ifdef ANDROID
#include <jni.h>
#include <mutex>
#include <memory>
namespace jni
{
    union UnionJNIEnv
    {
        JNIEnv* env;
        void* venv;

        UnionJNIEnv() { memset( this, 0, sizeof( *this ) ); }
    };


    void register_static_function(JavaVM* jvm, const std::string& className, const std::string& functionName, const std::string& functionSignature);
	std::tuple<jclass, jmethodID> get_static_function(const std::string& className, const std::string& functionName, const std::string& functionSignature);

	class CallGuard : std::unique_lock<std::mutex>
	{
	public:
		CallGuard();
		~CallGuard();

		static JNIEnv* env();
		static void setup(JNIEnv* env);

	private:
		static JavaVM* jvm;
		static UnionJNIEnv jnienv;
		static std::mutex mtx;
		bool needDetach = false;
	};

}

#endif




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