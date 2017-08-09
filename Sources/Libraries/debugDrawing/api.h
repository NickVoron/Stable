// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "gapi/library.include.h"
#include "immediateDraw/library.include.h"
#include "resourceShader/library.include.h"
#include "dxmath/library.include.h"

#include "types.h"

namespace DebugDrawing
{
	#pragma pack(push, 1)
	struct ShaderDataPerCamera
	{
		nm::matrix4 view;
		nm::matrix4 projection;
		Vector4 direction;
	};

	struct ShaderDataPerObject
	{
		ShaderDataPerObject();

		template<class ScaleType>
		void setWorld(const State& s, const ScaleType& scale)
		{
			dxmath::FillDXMatrix(world, s, scale);
		}

		D3DXMATRIX world;
		Vector4 color;
		int colorFromVertex[4];
	};
	#pragma pack(pop)

	template<gapi::Id api, bool triangles>
	struct DataDrawer
	{
		template<gapi::Id id, bool t, bool i> struct impl;

		template<> struct impl<gapi::DX9, true, true>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx9::im::DrawTriangleListIndexed(dd); } };
		template<> struct impl<gapi::DX9, true, false>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx9::im::DrawTriangleList(dd); } };
		template<> struct impl<gapi::DX9, false, true>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx9::im::DrawLineListIndexed(dd); } };
		template<> struct impl<gapi::DX9, false, false>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx9::im::DrawLineList(dd); } };

		template<> struct impl<gapi::DX11, true, true>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx11::im::DrawTriangleListIndexed(viewport, dd); } };
		template<> struct impl<gapi::DX11, true, false>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx11::im::DrawTriangleList(viewport, dd); } };
		template<> struct impl<gapi::DX11, false, true>		{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx11::im::DrawLineListIndexed(viewport, dd); } };
		template<> struct impl<gapi::DX11, false, false>	{ template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { dx11::im::DrawLineList(viewport, dd); } };

		template<class DrawData> static void draw(Viewports::Viewport& viewport, const DrawData& dd) { impl<api, triangles, DrawData::INDEXED_DATA>::draw(viewport, dd); }
	};

	struct ToolAPI
	{
		virtual void init(const char* fxFileName) = 0;
		virtual void uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data) = 0;
		virtual void uploadPerObject(Viewports::Viewport& viewport, const ShaderDataPerObject& data) = 0;
		virtual void begin(Viewports::Viewport& viewport, ShaderMode mode) = 0;
		virtual void end() = 0;
	};

	struct DrawableObject
	{
		virtual ~DrawableObject(){}

		void draw(Viewports::Viewport& viewport, ShaderMode mode)
		{
			tool->uploadPerObject(viewport, shaderData);
 			tool->begin(viewport, mode);
 			drawImpl(viewport);
 			tool->end();
		}

		virtual void drawImpl(Viewports::Viewport& viewport) = 0;

		ShaderDataPerObject shaderData;
		ToolAPI* tool;
	};

	struct RendererCacheEntry : public Base::IntrusiveList<RendererCacheEntry>::Node
	{
		virtual void clearData() = 0;
	};

	struct RendererCache : public Base::IntrusiveList<RendererCacheEntry>
	{
		void clearData()
		{
			for (RendererCacheEntry& entry : *this)
			{
				entry.clearData();
			}
		}

		static RendererCache& cache()
		{
			static RendererCache res;
			return res;
		}
	};


	template<class DrawData>
	struct DrawableObjectT : public DrawableObject
	{
		virtual void prepare(const DrawData& dd, bool updateDataBuffer) = 0;
		virtual void drawImpl(Viewports::Viewport& viewport) = 0;
	};

	template<class DrawData>
	struct RendererDeterminator
	{
		template<bool indexed>	struct impl
		{
			typedef dx11::Renderer<typename DrawData::Vertex, typename DrawData::IndexType> Renderer;
		};

		template<>	struct impl<false>	{ typedef dx11::VerticesRenderer<typename DrawData::Vertex> Renderer; };

		typedef typename impl<DrawData::INDEXED_DATA>::Renderer Renderer;
	};

	template<class DrawData, bool triangles, gapi::Id api> struct DrawableObjectAPI;

	template<class DrawData, bool triangles> 
	struct DrawableObjectAPI<DrawData, triangles, gapi::DX11> : public DrawableObjectT<DrawData>
	{	
		typedef typename RendererDeterminator<DrawData>::Renderer Renderer;

		struct RenderersMap : public std::map<const DrawData*, Renderer*>, public RendererCacheEntry
		{
			RenderersMap()	{	RendererCache::cache().push_back(*this);	}

			virtual void clearData()
			{			 
				Base::clearPtrContainer((std::map<const DrawData*, Renderer*>&)*this);
			}

			Renderer* get(const DrawData* dd)
			{
				Renderer*& rr = (*this)[dd];
				if (!rr)
				{
					rr = new Renderer();
					rr->init(*dd);
				}
				return rr;
			}
		};

		static RenderersMap& renderers()
		{
			static RenderersMap res;
			return res;
		}

		virtual void prepare(const DrawData& dd, bool updateDataBuffer)
		{
			renderer = renderers().get(&dd);

			if (updateDataBuffer)
			{
				renderer->init(dd);
			}			
		}

		virtual void drawImpl(Viewports::Viewport& viewport)
		{
 			dx11::DrawableObjectsList drawables;

			dx11::SetPrimitiveTopology<triangles ? gapi::TRIANGLELIST : gapi::LINELIST> topology;
			drawables.add(topology);

 			renderer->append(drawables);
 									
 			dx11::execute(viewport, drawables);
		}		
		
		 Renderer* renderer;
	};

	template<class DrawData, bool triangles>
	struct DrawableObjectAPI<DrawData, triangles, gapi::DX9> : public DrawableObjectT<DrawData>
	{
		virtual void prepare(const DrawData& dd, bool updateDataBuffer)
		{
			drawData = dd;
		}

		virtual void drawImpl(Viewports::Viewport& viewport)
		{
			DataDrawer<gapi::DX9, triangles>::draw(viewport, drawData);
		}

		DrawData drawData;
	};



	struct ToolDX11 : public ToolAPI
	{
		static const gapi::Id api = gapi::DX11;
		static const int shaderType = gapi::VERTEX_SHADER | gapi::PIXEL_SHADER;

		ShaderMode currentShaderMode;
		Resources::Shader<shaderType>* shader[INTERNAL_SHADERS_COUNT];

		virtual void init(const char* fxFileName);
		virtual void uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data);
		virtual void uploadPerObject(Viewports::Viewport& viewport, const ShaderDataPerObject& data);
		virtual void begin(Viewports::Viewport& viewport, ShaderMode mode);
		virtual void end();

		template<bool triangles, class DrawData>
		DrawableObject* object(const DrawData& dd, ShaderDataPerObject& shaderData, bool updateDataBuffer)
		{
			DrawableObjectAPI<DrawData, triangles, gapi::DX11>* obj = new DrawableObjectAPI<DrawData, triangles, gapi::DX11>();
			obj->prepare(dd, updateDataBuffer);
			obj->shaderData = shaderData;
			obj->tool = this;
			return obj;
		}

		dx11::ConstantBuffer cb0;
		dx11::ConstantBuffer cb1;
		dx11::RasterizerState rasterizerState;

		::dx11::DrawableObjectsList drawables;
	};


	struct ToolSwitcher
	{
		gapi::Id api;

		void init(const char* fxFileName, gapi::Id api);
 		void uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data);

		template<bool triangles, class DrawData> 
		DrawableObject* object(const DrawData& dd, ShaderDataPerObject& shaderData, bool updateDataBuffer)
		{
			DrawableObject* obj = 0;

			switch (api)
			{
			
			case gapi::DX11:	obj = toolDX11.object<triangles>(dd, shaderData, updateDataBuffer); break;
			}

			return obj;
		}

		
		ToolDX11 toolDX11;
	};

}









// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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