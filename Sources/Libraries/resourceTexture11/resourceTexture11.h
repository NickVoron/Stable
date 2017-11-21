#pragma once

#include "dx11/library.include.h"
#include "imageD3D/library.include.h"

#include "resourceManager/library.include.h"

namespace Resources
{ 
	struct TextureAPI11
	{
		void load(const char* fileName);
		void load(const char* fileName, int mipsCount);

		void loadCUBE(const dx11::CubemapDesc& desc);
		void clear();

		void createSampler();

		void load(stream::istream& is);
		void save(stream::ostream& os);

		void size(int& width, int& height);

		void append(dx11::DrawableObjectsList& drawables);

		dx11::Sampler sampler;
		dx11::Texture texture;
		dx11::ShaderResource resource;
	};


	struct TextureParams
	{
		int mipsCount = 1;
	};

	class Texture11 : public ResourceT<Texture11, TextureAPI11>
	{
	public:
		Texture11();
		inline static const char* basePath() { return "textures/"; }
		inline static const char* TypeName() { return "Texture"; }

		virtual void Clear();

		void Size(int& width, int& height);

	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;
	};
}