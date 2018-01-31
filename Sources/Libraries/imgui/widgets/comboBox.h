// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "list.h"

namespace imgui
{
	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount, int elementHeight);
	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount);
	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, int visibleElementsCount, int elementHeight);
	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, int visibleElementsCount);


	template<class ListContent> ListResult combobox(const char* caption, ListContent* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount, int elementHeight)	{ std::vector<ListElement> data; convertToListElements(data, elements, elementsCount); return combobox(caption, &data[0], elementsCount, pos, width, visibleElementsCount, elementHeight ); }
	template<class ListContent> ListResult combobox(const char* caption, ListContent* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount)						{ std::vector<ListElement> data; convertToListElements(data, elements, elementsCount); return combobox(caption, &data[0], elementsCount, pos, width, visibleElementsCount); }
	template<class ListContent> ListResult combobox(const char* caption, ListContent* elements, int elementsCount, int visibleElementsCount, int elementHeight)									{ std::vector<ListElement> data; convertToListElements(data, elements, elementsCount); return combobox(caption, &data[0], elementsCount, visibleElementsCount, elementHeight );	}
	template<class ListContent> ListResult combobox(const char* caption, ListContent* elements, int elementsCount, int visibleElementsCount)													{ std::vector<ListElement> data; convertToListElements(data, elements, elementsCount); return combobox(caption, &data[0], elementsCount, visibleElementsCount); }	

	template<class ListContent> ListResult combobox(const char* caption, const std::vector<ListContent>& elements, const Point2& pos, int width, int visibleElementsCount, int elementHeight)	{ return combobox(caption, &elements[0], elements.size(), pos, width, visibleElementsCount, elementHeight ); }
	template<class ListContent> ListResult combobox(const char* caption, const std::vector<ListContent>& elements, const Point2& pos, int width, int visibleElementsCount)						{ return combobox(caption, &elements[0], elements.size(), pos, width, visibleElementsCount); }
	template<class ListContent> ListResult combobox(const char* caption, const std::vector<ListContent>& elements, int visibleElementsCount, int elementHeight)									{ return combobox(caption, &elements[0], elements.size(), visibleElementsCount, elementHeight ); }
	template<class ListContent> ListResult combobox(const char* caption, const std::vector<ListContent>& elements, int visibleElementsCount)													{ return combobox(caption, &elements[0], elements.size(), visibleElementsCount); }

}//



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