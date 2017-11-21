#include "types.h"
#include "debugDrawing.h"

namespace DebugDrawing
{
	//void endDrawLinesImpl();


	namespace Primitives
	{
		//
		//
		//
		DrawableLists& DrawableLists::get()
		{
			static DrawableLists lists;
			return lists;
		}

		void DrawableLists::draw(std::list<DrawableObject*>& objects) const
		{
			for (auto& i : *this )
			{
				i.draw(objects);
			}

			DrawableObject* lines = endDrawLinesImpl();
			if (lines)
			{
				objects.push_back(lines);
			}			
		}

		void DrawableLists::reset()
		{
			for (auto& i : *this)
			{
				i.reset();
			}
		}
	}	
}