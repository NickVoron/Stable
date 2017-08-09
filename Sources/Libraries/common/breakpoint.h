// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
#endif 
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