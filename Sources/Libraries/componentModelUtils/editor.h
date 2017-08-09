// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#ifdef WIN32
#include "imguiModules/library.include.h"
#endif
#include "componentModel/library.include.h"

namespace ComponentsEditor
{
	struct editor_int : public PropertyEditor<int>
	{
		virtual void edit(const char* name, int& val);
	};

	struct editor_bool : public PropertyEditor<bool>
	{
		virtual void edit(const char* name, bool& val);
	};

	struct editor_float : public PropertyEditor<float>
	{
		virtual void edit(const char* name, float& val);
	};

	struct editor_color : public PropertyEditor< image::Color<image::RGBA32F> >
	{
		virtual void edit(const char* name, image::Color<image::RGBA32F>& val);
	};

	struct editor_vector3 : public PropertyEditor<Vector3>
	{
		virtual void edit(const char* name, Vector3& val);
	};

	void edit(EntityData* entity);
	void init(); 

	struct EntityEditor 
#ifdef WIN32
		: public imgui::modules::Module
#endif
	{
		EntityEditor();

		virtual void process();
		virtual const char* name() const;

		bool active;
		EntityData* currentEntity;
		EntitiesList* entities;
	};
}







// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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