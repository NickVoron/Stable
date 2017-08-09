// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "streaming/library.include.h"
#include "stuff/library.include.h"

namespace Resources 
{
	class IOStreamsManager
	{
	public:
		static stream::ostream* writer(const char* resourceName);
		static stream::istream* reader(const char* resourceName);

		static void close(stream::ostream* os);
		static void close(stream::istream* is);
	};

	template<class ResourceType>
	class BaseResourceLoader : public Streaming::LoaderBase
	{
	public:
		struct Params
		{
			const void* userData = 0;
			const char* sourceFileName;
			const char* compiledFileName;
			uint32_t userDataSize = 0;
			ResourceType* resourceToLoad = 0;
		};

		BaseResourceLoader(const Params& p) : params(p)
		{
			ENFORCE(params.resourceToLoad);
		}

		virtual void remove()
		{
			ENFORCE(params.resourceToLoad);
			params.resourceToLoad->RemoveFromSet();
			
		}

	protected:
		void loadCompiled(ResourceType* resource, const char* fileName)
		{
			ENFORCE(resource);

			stream::istream* is = IOStreamsManager::reader(fileName);

			resource->LoadCompiled(*is);
			resource->OnLoadCompiledComplete();

			IOStreamsManager::close(is);
		}

		void compile(ResourceType* resource, const char* fileName, const void* userData, unsigned int userDataSize)
		{
			ENFORCE(resource);

			resource->LoadSource(resource->sourceFileName.c_str(), userData, userDataSize);
			resource->UpdateCompiled();
			resource->Clear();
		}

		virtual bool isSame(Streaming::LoaderBase* loader) const
		{
			return false;
		}

		virtual std::string desc() const { return params.sourceFileName; }

		Params params;
	};

	template<class ResourceType>
	class CompiledResourceLoader : public BaseResourceLoader<ResourceType> 
	{
	public:
		CompiledResourceLoader(const typename BaseResourceLoader<ResourceType>::Params& p) : BaseResourceLoader<ResourceType>(p)
		{
			ENFORCE(this->params.resourceToLoad);
		}

		virtual void load()
		{
			CompiledResourceLoader::loadCompiled(this->params.resourceToLoad, this->params.compiledFileName);
		}

		virtual bool isSame(Streaming::LoaderBase* loader) const
		{
			CompiledResourceLoader* l = dynamic_cast<CompiledResourceLoader*>(loader);
			if(!l)
				return false;
			bool res = this->params.resourceToLoad == l->params.resourceToLoad;
			if(res) 
				LOG_MSG("resources is same");
			return res;
		}
	};

	template<class ResourceType>
	class SourceResourceLoader : public BaseResourceLoader<ResourceType>
	{
	public:
		SourceResourceLoader(const typename BaseResourceLoader<ResourceType>::Params& p) : BaseResourceLoader<ResourceType>(p)
		{
			ENFORCE(this->params.resourceToLoad);
		}

		virtual void load()
		{
			try_execute([this] {
				SourceResourceLoader::compile(this->params.resourceToLoad, this->params.sourceFileName, this->params.userData, this->params.userDataSize);
				SourceResourceLoader::loadCompiled(this->params.resourceToLoad, this->params.compiledFileName);
			});
		}

	};

}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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