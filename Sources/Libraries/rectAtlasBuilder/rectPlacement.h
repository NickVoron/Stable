// --------------------------------------------------------------------------------
// Name        : RectPlacement.h
// Description : A class that allocates subrectangles into power-of-2 rectangles
//               (C) Copyright 2000-2002 by Javier Arevalo
//               This code is free to use and modify for all purposes
// --------------------------------------------------------------------------------
#pragma once

#include <vector>

#include "unigui/rect.h"

//  static bool Greater(const TRect &a, const TRect &b) { return a.w*a.h > b.w*b.h; }     // Greater rect area. Not as good as the next heuristic

template<class T>
bool greater(const unigui::math::Rect<T> &a, const unigui::math::Rect<T> &b) { return (a.size.x > b.size.x && a.size.x > b.size.y) || (a.size.y > b.size.x && a.size.y > b.size.y); }  // Greater size in at least one dim.


class CRectPlacement
{
public:
	typedef unigui::math::Point2<int> TPos;
	typedef unigui::math::Rect<int> TRect;

	// ---------------------

	typedef std::vector<TPos>  CPosArray;
	typedef std::vector<TRect> CRectArray;

	// ---------------------

	CRectPlacement()                    { Init(); }
	~CRectPlacement()                   { End(); }

	void      Init    (int w = 1, int h = 1);
	void      End     ();
	bool      IsOk    ()                      const { return m_size.size.x > 0; }

	int       GetW    ()                      const { return m_size.size.x; }
	int       GetH    ()                      const { return m_size.size.y; }
	long      GetArea ()                      const { return m_area; }
	long      GetTotalArea ()                 const { return m_size.square(); }

	bool AddAtEmptySpotAutoGrow (TRect *pRect, int maxW, int maxH);

private:
	TRect       m_size;
	CRectArray  m_vRects;
	CPosArray   m_vPositions;
	long        m_area;

	// ---------------------

	bool IsFree                 (const TRect &r) const;
	void AddPosition            (const TPos &p);
	void AddRect                (const TRect &r);
	bool AddAtEmptySpot         (TRect &r);
};