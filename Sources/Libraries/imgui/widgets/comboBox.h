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