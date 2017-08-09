// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "baseRenderer.h"
#include "../types.h"

namespace unigui {
	namespace draw {

		template<class InstanceVertex, class MeshVertex, class Index, int maxElements>
		class BaseRendererAPI : public RendererUsingInterface
		{
		public:
			typedef BaseRendererAPI BaseRendererImpl;

			BaseRendererAPI()
			{
				if (gapi::initialized(gapi::DX9))
				{
					impl = new BaseRenderer9<InstanceVertex, MeshVertex, Index, ::dx9::DrawIndexedPrimitive, maxElements>();
				}
				else if (gapi::initialized(gapi::DX11))
				{
					impl = new BaseRenderer11<InstanceVertex, MeshVertex, Index, maxElements>();
				}
			}

			~BaseRendererAPI()
			{
				delete impl;
			}

			virtual void init() = 0;
			virtual void prepare() = 0;
			virtual void finish() = 0;

			void draw(int count) { impl->draw(count); }
			void begin() { impl->begin(); }
			void end() { impl->end(); }
			void unlock() { impl->unlock(); }
			void lock() { impl->lock(); }
			void reset() { impl->reset(); }

			RendererUsingInterface& get() { return *impl; }

			int& cursor() { return impl->cursor; }
			Vector2& invsize() { return impl->invsize; }

			void checkedBegin()
			{
				if (cursor() < 0)
				{
					init();
					reset();
					lock();
				}
			}

			void checkedFlush()
			{
				
				{
					unigui::draw::flush();
				}
			}

			void flush()
			{
				prepare();
				flushCore();
				finish();
			}

			void flushCore()
			{
				if (cursor() >= 0)
				{
					unlock();
					begin();
					draw(cursor());
					end();

					cursor() = -1;
				}
			}

			VertexStreamBase<InstanceVertex>& instances() { return impl->instances(); }

		protected:
			void init(const VertexStream<MeshVertex>& meshVS, const IndexStream<Index>& meshIS)
			{
				impl->init(meshVS, meshIS);
				inited = true;
			}

			BaseRenderer<InstanceVertex, MeshVertex, Index, maxElements>* impl;
			bool inited = false;
		};

	}
}




// Copyright (C) 2015-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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