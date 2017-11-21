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
