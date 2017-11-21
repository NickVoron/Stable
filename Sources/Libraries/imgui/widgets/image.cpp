#include "image.h"
#include "../imgui.h"
#include "../controlTypes.h"
#include "../globals.h"

#include "uniguidraw/library.include.h"

namespace imgui
{

	std::vector<multiapi::Texture*> textures;

	namespace core
	{
		void clear_image_cache()
		{
			textures.clear();
		}
	}
	
	
	void image(const Point2& pos, const StdImage& img)
	{
		image(pos, Point2(img.sizeX, img.sizeY), img);
	}

	void image(const Point2& pos, const Point2& size, const StdImage& img)
	{
		if (!img.empty())
		{
			textures.emplace_back(new multiapi::Texture());
 			auto texture = textures.back();
 			texture->create(img);

			unigui::Rect rect(pos.x, pos.y, size.x, -size.y);
			unigui::draw::texrect(rect, texture);
			//unigui::draw::renderer_clear(texture);
		}
	}

	void image(const StdImage& img)
	{
		image(Point2(0,0), img);
	}

	void image(const Vector3& pos, const StdImage& img)
	{
		Point2 p;
		if (detail::worldToScreen(pos, p))
		{
			image(p, img);
		}
	}

	void image(const Vector3& pos, float worldWidth, const StdImage& img)
	{
		Point2 p, c;
		if (detail::worldToScreen(pos, p))
		{
			Vector2 size(worldWidth, ((float)img.sizeY / img.sizeX) * worldWidth);
			Vector3 corner = detail::billboardPosition(pos, size);
			detail::worldToScreen(corner, c);
			image(p, c - p, img);
		}
	}

}//