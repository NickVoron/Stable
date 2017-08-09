// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#ifndef TRI_STRIPPER_HEADER_GUARD_TYPES_H
#define TRI_STRIPPER_HEADER_GUARD_TYPES_H




namespace triangle_stripper {

	namespace detail {




class triangle
{
public:
	triangle() { }
	triangle(index A, index B, index C)
		: m_A(A), m_B(B), m_C(C), m_StripID(0) { }

	void ResetStripID()							{ m_StripID = 0; }
	void SetStripID(size_t StripID)				{ m_StripID = StripID; }	
	size_t StripID() const						{ return m_StripID; }

	index A() const								{ return m_A; }
	index B() const								{ return m_B; }
	index C() const								{ return m_C; }
	
private:
	index	m_A;
	index	m_B;
	index	m_C;

	size_t	m_StripID;
};



class triangle_edge
{
public:
	triangle_edge(index A, index B)
		: m_A(A), m_B(B) { }

	index A() const								{ return m_A; }
	index B() const								{ return m_B; }

	bool operator == (const triangle_edge & Right) const {
		return ((A() == Right.A()) && (B() == Right.B()));
	}

private:
	index	m_A;
	index	m_B;
};



enum triangle_order { ABC, BCA, CAB };



class strip
{
public:
	strip()
		: m_Start(0), m_Order(ABC), m_Size(0) { }

	strip(size_t Start, triangle_order Order, size_t Size)
		: m_Start(Start), m_Order(Order), m_Size(Size) { }

	size_t Start() const						{ return m_Start; }
	triangle_order Order() const				{ return m_Order; }
	size_t Size() const							{ return m_Size; }

private:
	size_t			m_Start;
	triangle_order	m_Order;
	size_t			m_Size;
};




	} 

} 




#endif 




// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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