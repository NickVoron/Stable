#pragma once

#include "baseRendererAPI.h"
#include "textureSetter.h"

namespace unigui 
{
	namespace draw 
	{

		//
		struct StatesBucketBase;
		struct Renderers : public std::vector<StatesBucketBase*>
		{
			void flush();
		};

		Renderers& renderers();
		
		//BaseRectRenderer
		template<class InstVtx>
		class BaseRectRenderer : public BaseRendererAPI<InstVtx, MeshVtx, unsigned short, 4096>
		{
		protected:
			virtual void init()
			{
				if (!this->inited)
				{
					VertexStream<MeshVtx> vs;
					IndexStream<unsigned short> is;

					Private::initQuadPrimitive(vs, is);
					BaseRendererAPI<InstVtx, MeshVtx, unsigned short, 4096>::init(vs, is);
				}			
			}
		};

		//TexRectRenderer
		class TexRectRenderer : public BaseRectRenderer<TRInstVtx>
		{
		public:
			virtual void add(const math::Rect<float>& r, const math::Rect<float>& tr, const Color& color);
		};

		//SolidRectRenderer
		class SolidRectRenderer : public BaseRectRenderer<RInstVtx>
		{
		public:
			virtual void add(const math::Rect<float>& r, const Color& color);
		};

		//LineRenderer
		class LineRenderer : public BaseRectRenderer<LineInstVtx>
		{
		public:
			virtual void add(const math::Rect<float>& r, const math::Point2<float>& p0, const math::Point2<float>& p1, float thickness, const Color& color);
		};

	}//
}//
