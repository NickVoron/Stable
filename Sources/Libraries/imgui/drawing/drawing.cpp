#include "drawing.h"

#include "../types.h"

#include "debugDrawing/library.include.h"
#include "geometry/library.include.h"

#include "font/library.include.h"
#include "fontDraw/library.include.h"
#include "unigui/library.include.h"
#include "uniguidraw/library.include.h"

namespace imgui
{
	namespace detail
	{
// 		using namespace Geometry;
// 		typedef Vertex<TYPELIST_2(POST, UV2<0>)> Vtx;
// 		typedef VertexStream<Vtx> VS;
// 		typedef IndexStream<unsigned int> IS;
// 		typedef Mesh<Vtx, unsigned int> MS;
// 
// 		MS mesh;
// 		dx9::DynamicMesh drawer;
// 
// 		Resources::Effect* effectFX;
// 
		extern Resources::Font* font;
	}

namespace drawing{

	using namespace detail;

	void drawRect(const Rect& rect, const Color& color)
	{
		unigui::draw::rect(rect, color);
// 		Resources::Effect::ClientResource e = effectFX->GetResource();
// 		if(e)
// 		{
// 			Geometry::Generation::generateScreenQuad(detail::mesh, Vector2(rect.pos.x, rect.pos.y), Vector2(rect.size.x, rect.size.y));
// 			detail::drawer.uploadMesh(detail::mesh);
// 
// 			e->SetValue("color", &color, sizeof(color));
// 			e->Begin(0,0);
// 			e->BeginPass(0);
// 			detail::drawer.draw();
// 			e->EndPass();
// 			e->End();
// 		}		
	}

	//void drawLine(const types::Point2& start, const types::Point2& end, const types::Color& color)
	//{
	//	//DebugDrawing::color( Vector3(color.r, color.g, color.b) );
	//	DebugDrawing::color( Vector3(1, 0,0) );
	//	//DebugDrawing::line(nm::index2(start.x, start.y), nm::index2(end.x, end.y) );
	//	DebugDrawing::line(nm::index2(0,0), nm::index2(1000, 1000) );
	//}

/*
	void drawText(const char* text, const Point2& pos, const Color& color)
	{
		font.print()
		//DebugDrawing::color( Vector3(color.r, color.g, color.b) );
		//DebugDrawing::text(nm::index2(pos.x, pos.y), text);
	}*/


// 	void drawText(const wchar_t* text, const Rect& rect, const Color& color)
// 	{
// 		if (font->GetResource())
// 		{
// 			font->GetResource()->print(text, rect, color);
// 		}
// 		
// 	}
// 
// 	void drawText(const char* text, const Rect& rect, const Color& color)
// 	{
// 		drawText( Base::StrUtils::Convert(text).c_str(), rect, color );
// 	}


	void drawLineRect(const Rect& rect, const Color& color)
	{
		unigui::draw::linerect(rect, color);
// 		drawRect( Rect(rect.pos, Point2(2, rect.size.y)), color);
// 		drawRect( Rect(rect.pos, Point2(rect.size.x, 2)), color);
// 
// 		drawRect( Rect(Point2(rect.pos.x, rect.pos.y + rect.size.y-2), Point2(rect.size.x, 2)), color);
// 		drawRect( Rect(Point2(rect.pos.x+ rect.size.x-2, rect.pos.y),  Point2(2, rect.size.y)), color);
	}
}
}
