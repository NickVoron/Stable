#pragma once

#include "stuff/callDispatcher.h"

template<class Type>
struct PropertyEditor
{
	typedef Type UserStruct;
	virtual void edit(const char* name, Type& val) = 0;
};

struct PropertyEditors : public TemplateCallDispatcher<PropertyEditor> 
{
	template<class T>
	static void edit(const char* name, T& value)
	{
		if(PropertyEditor<T>* editor = ptr<T>())
		{
			editor->edit(name, value);
		}
		else
		{
			LOG_ERROR("editor for type: " << typeid(T).name() << " not registred" );
		}
	}
};