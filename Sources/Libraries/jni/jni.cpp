// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "jni.h"

#ifdef ANDROID

#include <map>
#include "stuff/library.include.h"

namespace jni
{
	namespace
	{
		std::mutex jnimtx;
		std::map<std::string, std::tuple<jclass, jmethodID>> staticMethods;
	}

	void register_static_function(JavaVM* jvm, const std::string& className, const std::string& functionName, const std::string& functionSignature)
	{
		std::unique_lock<std::mutex> lock;

		if (jvm)
		{
			bool needDetach = false;
			void* env;
			JNIEnv* penv = nullptr;
			auto res = jvm->GetEnv(&env, JNI_VERSION_1_6);
			if (res == JNI_EDETACHED || res == JNI_EVERSION)
			{
				res = jvm->AttachCurrentThread(&penv, NULL);
				env = penv;
				needDetach = true;
			}

			if (res == JNI_OK && env)
			{
				auto cls = ((JNIEnv*) env)->FindClass(className.c_str());
				auto myClass = (jclass) ((JNIEnv*) env)->NewGlobalRef(cls);

				if (auto method = ((JNIEnv*) env)->GetStaticMethodID(myClass, functionName.c_str(), functionSignature.c_str()))
				{
					staticMethods[str::stringize(className, functionName, functionSignature).str()] = std::make_tuple(myClass, method);
				}
				else
				{
					LOG_ERROR("JNI: " << functionName << "<" << functionSignature << ">" << "not found");
				}
			}
			else
			{
				LOG_ERROR("JNI: " << className << " not found");
			}

			if (needDetach)
			{
				jvm->DetachCurrentThread();
			}
		}
	}

	std::tuple<jclass, jmethodID> get_static_function(const std::string& className, const std::string& functionName, const std::string& functionSignature)
	{
		std::unique_lock<std::mutex> lock;
		auto strid = str::stringize(className, functionName, functionSignature).str();
		auto it = staticMethods.find(strid);
		return (it != staticMethods.end()) ? it->second : std::tuple<jclass, jmethodID>(nullptr, nullptr);
	}

    UnionJNIEnv CallGuard::jnienv;

	JavaVM* CallGuard::jvm = nullptr;
	std::mutex CallGuard::mtx;

	JNIEnv* CallGuard::env()
	{
		return jnienv.env;
	}

	void CallGuard::setup(JNIEnv* env)
	{
		jnienv.env = env;
		env->GetJavaVM(&jvm);
	}

	CallGuard::CallGuard() : std::unique_lock<std::mutex>(mtx)
	{
		if (jvm)
		{
            UnionJNIEnv env;
			auto res = jvm->GetEnv(&env.venv, JNI_VERSION_1_6);
			if ((needDetach = (res == JNI_EDETACHED || res == JNI_EVERSION)))
			{
				res = jvm->AttachCurrentThread(&env.env, NULL);
			}
		}
	}

	CallGuard::~CallGuard()
	{
		if (jvm && needDetach)
		{
			jvm->DetachCurrentThread();
		}
	}
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