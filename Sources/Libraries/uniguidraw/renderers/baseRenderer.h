// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "dx9/library.include.h"
#include "dx11/library.include.h"
#include "unigui/library.include.h"
#include "curve/library.include.h"
#include "resourceTexture/library.include.h"
#include "resourceAtlas/library.include.h"
#include "resourceFont/library.include.h"

#include "immediateDraw/library.include.h"
#include "../types.h"

namespace unigui {
	namespace draw {

		template<class InstanceVertex, class MeshVertex, class Index, int maxElements>
		class BaseRenderer : public RendererUsingInterface
		{
		public:
			BaseRenderer()	{ cursor = -1;}
			virtual ~BaseRenderer()	{ }

			void reset()	{ cursor = 0; }

			virtual void draw(int count) = 0;

			virtual void begin() = 0;
			virtual void end() = 0;

			virtual void unlock() = 0;
			virtual void lock() = 0;

			virtual VertexStreamBase<InstanceVertex>& instances() = 0;
			virtual void init(const VertexStream<MeshVertex>& meshVS, const IndexStream<Index>& meshIS) = 0;

			Vector2 invsize;
			int cursor;
		};


		
		template<class InstanceVertex, class MeshVertex, class Index, class DIP, int maxElements>
		class BaseRenderer9 : public BaseRenderer<InstanceVertex, MeshVertex, Index, maxElements>
		{
		public:
			::dx9::InstancedRenderer<InstanceVertex, MeshVertex, Index, DIP> renderer9;

			void draw(int count)
			{
				renderer9.draw(count);
			}

			void lock() { renderer9.lock(maxElements); }
			void unlock() { renderer9.unlock(); }
			void begin() { renderer9.begin(); }
			void end() { renderer9.end(); }

			VertexStreamBase<InstanceVertex>& instances()
			{
				return renderer9.instances();
			}

		protected:
			void init(const VertexStream<MeshVertex>& meshVS, const IndexStream<Index>& meshIS)
			{
				renderer9.init(meshVS, meshIS, true);
			}
		};

		
		template<class InstanceVertex, class MeshVertex, class Index, int maxElements>
		class BaseRenderer11 : public BaseRenderer<InstanceVertex, MeshVertex, Index, maxElements>
		{
		public:
			::dx11::InstancedRenderer<InstanceVertex, MeshVertex, Index> renderer11;
			Geometry::VertexStream<InstanceVertex> instancesVS;


			void draw(int count)
			{
				drawImpl(count);
			}

			void lock() { instancesVS.resize(maxElements); }
			void unlock() { renderer11.instances(instancesVS); }

			void begin() {	}
			void end() {	}

			VertexStreamBase<InstanceVertex>& instances()
			{
				return instancesVS;
			}

		private:
			void drawImpl(int count)
			{
				Viewports::Viewport* v = dx11::currentViewport.viewport();

				if (v)
				{
					::dx11::DrawableObjectsList drawables;
					renderer11.setup(count);
					renderer11.append(drawables);
					::dx11::execute(*v, drawables);
				}
			}

		protected:

			void init(const VertexStream<MeshVertex>& meshVS, const IndexStream<Index>& meshIS)
			{
				renderer11.init(meshVS, meshIS);
			}

			int cursor;
		};


	}
}//



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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