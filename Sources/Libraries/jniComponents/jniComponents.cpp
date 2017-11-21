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
