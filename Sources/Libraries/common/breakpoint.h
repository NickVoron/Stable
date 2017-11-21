#pragma once

namespace Base
{
	namespace Debug
	{
		struct DefaultPredicate { template <class T> inline static bool Right(const T& obj) { return obj; } };

		struct DefaultRaiser
		{
			template <class T>
			inline static void Throw(const T&, const char* locus)
			{
#ifdef WIN32
				if (IsDebuggerPresent())
				{
					__debugbreak();
				}
#endif // WIN32				
			}
		};

		template<typename Ref, typename P, typename R>
		class Breakpointer
		{
		public:
			inline Breakpointer(Ref t, const char* locus) : t_(t), locus_(P::Right(t) ? locus : 0) { }

			inline Ref operator*() const
			{
				if (locus_)
				{
					R::Throw(t_, locus_);
				}

				return t_;
			}

		private:
			Ref t_;
			const char* const locus_;
		};

		template <class P, class R, typename T> inline auto MakeBreakpointer(const T& t, const char* locus) { return Breakpointer<const T&, P, R>(t, locus); }
		template <class P, class R, typename T> inline auto MakeBreakpointer(T& t, const char* locus) { return Breakpointer<T&, P, R>(t, locus); }

	}
}

#define BREAKPOINT(exp)	*Base::Debug::MakeBreakpointer<Base::Debug::DefaultPredicate, Base::Debug::DefaultRaiser>( (exp), SOURCE_LOCATION_STR(exp));
