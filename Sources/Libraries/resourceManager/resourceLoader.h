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
			//SAFE_DELETE(params.resourceToLoad);
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

		void compile(ResourceType* resource, const char* fileName, const void* userData)
		{
			ENFORCE(resource);

			resource->LoadSource(resource->sourceFileName.c_str(), (UserData*)userData);
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
				SourceResourceLoader::compile(this->params.resourceToLoad, this->params.sourceFileName, this->params.userData);
				SourceResourceLoader::loadCompiled(this->params.resourceToLoad, this->params.compiledFileName);
			});
		}

	};

}
