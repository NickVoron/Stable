#pragma once

#include "resourceManager/library.include.h"

#include "dx11/library.include.h"

namespace Resources
{
	class ShaderInterface
	{
	public: 
		virtual bool set(Viewports::Viewport& viewport)=0;
	};

	template<int shaders>
	class Shader : public ResourceT<Shader<shaders>, dx11::ShaderSet<shaders>>, public ShaderInterface
	{
	public:
		inline static const char* basePath() { return "shaders/"; }
		inline static const char* TypeName() { return "Shader"; }

		virtual void Clear();

		virtual bool set(Viewports::Viewport& viewport);

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFileName, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
	};

	//
	//
	//
	template<int shaders>
	void Shader<shaders>::Clear()
	{
		this->deactivate();
		this->nativeResource.clear();
	}

	template<int shaders>
	void Shader<shaders>::Compile(stream::ostream& os)
	{
		this->nativeResource.save(os);
	}

	template<int shaders>
	void Shader<shaders>::LoadSource(const char* sourceFile, const UserData* userData)
	{
		if(this->nativeResource.compile(Base::StrUtils::Convert( sourceFile ).c_str(), userData))
		{
			this->activate();
		}
		else
		{
			this->deactivate();
		} 		
	}

	template<int shaders>
	void Shader<shaders>::LoadCompiled(stream::istream& is)
	{
		this->nativeResource.load(is);
		this->activate();
	}

	template<int shaders>
	bool Shader<shaders>::set(Viewports::Viewport& viewport)
	{
		return this->resource([&viewport](auto& shader) { dx11::execute(viewport, shader); });
	}

	typedef Shader<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER> VertexPixelShader;

//	template<int shaders> bool has_external_editor<Shader<shaders>>() { return true; }
}