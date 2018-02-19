// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "035.complexLinks.h"
#include "commonComponents/library.include.h"
#include "utils.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{
	class ComplexReference : public SimpleComponentAutoLink<ComplexReference>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ComplexReference);
		struct Resource : public ResourceBase
		{
			void properties(ComplexReference& component)
			{
				property("reference", component.reference);
			}
		};

		ComponentLink<Position, LinearMover> reference;		
	};

	class ReferenceAssigner : public UpdatableComponentAutoLink<ReferenceAssigner, ComplexReference>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(ReferenceAssigner);
		
		void update()
		{
			auto& reference = component<ComplexReference>().reference;

			referenceLP = reference;
			referenceP = reference;
			referenceL = reference;

			referenceLP([](auto& mover, auto& pos)
			{
				LOG_EXPRESSION(typeid(mover).name(), typeid(pos).name());
			});

			referenceP([](auto& pos)
			{
				LOG_EXPRESSION(typeid(pos).name());
			});

			referenceL([](auto& mover)
			{
				LOG_EXPRESSION(typeid(mover).name());
			});
			
		}

		ComponentLink<LinearMover, Position> referenceLP;
		ComponentLink<Position> referenceP;
		ComponentLink<LinearMover> referenceL;
	};

	ComplexLinks::ComplexLinks()
	{
		ComponentModel::components<ComplexReference, ReferenceAssigner>();

		try
		{
			std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/035.complexLinks.desc");
			ComponentModel::descriptionTest(path.c_str());
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
		}
	}

}
#endif



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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