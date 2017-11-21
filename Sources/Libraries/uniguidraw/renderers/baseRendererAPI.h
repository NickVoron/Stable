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
				//if (cursor() >= maxElements - 1)
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

	}//
}//
