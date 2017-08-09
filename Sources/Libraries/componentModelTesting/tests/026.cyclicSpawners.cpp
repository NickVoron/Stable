// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "026.cyclicSpawners.h"
#include "utils.h"
#include "quietConfigurator.h"
#include "componentLinkModelConfigurator.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"


namespace
{
	static std::string result;
}

namespace ComponentModelTesting
{					   
	class CyclicS1;
	class CyclicS2;
	class CyclicS3;
	class CyclicS4;
	class CyclicS5;

	class CyclicS1 : public UpdatableComponentAutoLink<CyclicS1, ConfigurableSpawner<CyclicS2>>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CyclicS1);
		CyclicS1() { result += "1"; }
		void update() 
		{ 
			
			spawn([](CyclicS2& component) {}); 
		}
	};

	class CyclicS2 : public UpdatableComponentAutoLink<CyclicS2, ConfigurableSpawner<CyclicS3>>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CyclicS2);
		CyclicS2() { result += "2"; }
		void update() 
		{
			
			spawn([](CyclicS3& component) {});
		}
	};

	class CyclicS3 : public UpdatableComponentAutoLink<CyclicS3, ConfigurableSpawner<CyclicS4>>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CyclicS3);

		CyclicS3() { result += "3"; }
		void update() 
		{
			
			spawn([](CyclicS4& component) {});
		}

	};

	class CyclicS4 : public UpdatableComponentAutoLink<CyclicS4, ConfigurableSpawner<CyclicS5>>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CyclicS4);

		CyclicS4() { result += "4"; }
		void update() 
		{
			
			spawn([](CyclicS5& component) {});
		}
	};

	class CyclicS5 : public UpdatableComponentAutoLink<CyclicS5, ConfigurableSpawner<CyclicS1>>
	{
	public:
		CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(CyclicS5);

		CyclicS5() { result += "5"; }
		void update() 
		{
			
			spawn([](CyclicS1& component) {});
		}
	};

CyclicSpawners::CyclicSpawners()
{
	try
	{
		ComponentsCore::components<CyclicS1, CyclicS2, CyclicS3, CyclicS4, CyclicS5>();
		result.clear();
		std::string path = Resources::resourceRelativePath("desc/cm2Testing/1.basic/026.cyclicSpawners.desc");
		ComponentModel::descriptionTest(path.c_str(), 100,
		[](auto&) { result.clear(); }, 
		[](auto&) 
		{ 

		},
		[](auto index, auto& entities)
		{


		},
		[](auto index, auto& entities) 
		{

		});
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e);
	}
}

}//



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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