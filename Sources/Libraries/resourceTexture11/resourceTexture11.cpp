// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "resourceTexture11.h"
#include "image/library.include.h"


namespace Resources
{ 
	void TextureAPI11::load(const char* fileName)
	{
		texture.create<image::RGBA8>(fileName);
					
		ZeroMemory(&resource.desc, sizeof(resource.desc));
		resource.desc.Format = image::dx11::image_to_d3d<image::RGBA8>::d3dFmt;
		resource.desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource.desc.Texture2D.MostDetailedMip = 0;
		resource.desc.Texture2D.MipLevels = 1;

		createSampler();
	}


	void TextureAPI11::load(const char* fileName, int mipsCount)
	{
		mipsCount = texture.createWithMips<image::RGBA8>(fileName, mipsCount);

		ZeroMemory(&resource.desc, sizeof(resource.desc));
		resource.desc.Format = image::dx11::image_to_d3d<image::RGBA8>::d3dFmt;
		resource.desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource.desc.Texture2D.MostDetailedMip = 0;
		resource.desc.Texture2D.MipLevels = mipsCount;

		createSampler();
	}


	void TextureAPI11::loadCUBE(const dx11::CubemapDesc& desc)
	{
		texture.create<image::RGBA8>(desc);

		ZeroMemory(&resource.desc, sizeof(resource.desc));
		resource.desc.Format = image::dx11::image_to_d3d<image::RGBA8>::d3dFmt;
		resource.desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		resource.desc.Texture2D.MostDetailedMip = 0;
		resource.desc.Texture2D.MipLevels = 1;

		createSampler();
	}

	void TextureAPI11::createSampler()
	{
		sampler.samplerDesc.addressU = gapi::TextureAddressMode::WRAP;
		sampler.samplerDesc.addressV = gapi::TextureAddressMode::WRAP;
		sampler.samplerDesc.filter = gapi::MIN_MAG_MIP_LINEAR;
		sampler.samplerDesc.maxAnisotropy = 1;

		sampler.create();
	}

	void TextureAPI11::clear()
	{
	}

	void TextureAPI11::save(stream::ostream& os)
	{
		texture.save(os);
		sampler.save(os);
		resource.save(os);
	}

	void TextureAPI11::load(stream::istream& is)
	{
		clear();
		texture.load(is);
		sampler.load(is);
		resource.load(is);
		resource.create(texture);
	}


	void TextureAPI11::append(dx11::DrawableObjectsList& drawables)
	{
		drawables.add(resource);
		drawables.add(sampler);
	}

	void TextureAPI11::size(int& width, int& height)
	{
		texture.size(width, height);
	}

	
	
	
	Texture11::Texture11()
	{

	}

	void Texture11::Clear()
	{
		deactivate();
		nativeResource.clear();
	}

	void Texture11::Compile(stream::ostream& os)
	{
		nativeResource.save(os);
	}
	
	void Texture11::LoadSource(const char* sourceFile, const void* userData, std::size_t userDataSize)
	{
		std::string fileName = sourceFile;
		std::string fileExt = Base::FileUtils::GetLastExtention(fileName);

		if (fileExt == "cube")
		{
			typedef std::vector<str::string256> SrcList;

			SrcList files;

			boost::filesystem::path folderPath = fileName;
			folderPath.remove_filename();

			std::ifstream file(fileName.c_str());
			ENFORCE(file.good());

			char buffer[4096];
			while (file.good())
			{
				file.getline(buffer, 4096);
				str::string256 full = folderPath.string().c_str();

				full += "/";
				full += buffer;

				bool is_file = boost::filesystem::exists(full.c_str());
				

				if (is_file)
				{
					files.push_back(full);
					dependencies.addActualDependence(full.c_str(), buffer);
				}
			}

			ENFORCE(files.size() == 6);
			dx11::CubemapDesc cube;
			for (int i = image::POSITIVE_X; i < image::FACE_PLANES_COUNT; ++i)
			{
				cube.planes[i] = files[i];
			}

			nativeResource.loadCUBE(cube);
		}
		else
		{
			const TextureParams* params = 0;
			if (userDataSize == sizeof(TextureParams))
			{
				params = reinterpret_cast<const TextureParams*>(userData);
			}

			nativeResource.load(sourceFile, params ? params->mipsCount : 0);
		}	
	}

	void Texture11::LoadCompiled(stream::istream& is)
	{
		nativeResource.load(is);
		activate();
	}

	void Texture11::Size(int& width, int& height)
	{
		width = 0;
		height = 0;
		resource([&width, &height](auto res) { res.size(width, height); });
	}
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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