// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "rectAtlas/library.include.h"
#include "image/library.include.h"
#include "imageD3D/library.include.h"
#include "stream/library.include.h"
#include "stuff/library.include.h"

#include "rectPlacement.h"

#ifdef STREAM_IO_LIBRARY

class TextureAtlasComposer
{
public:
	typedef image::Plane< image::Color<image::RGBA32F> > AtlasImage;
	typedef image::Plane< image::Color<image::R32F> > AtlasAlpha;
	
	struct RectSource
	{
		RectSource();

		void addMemBlock(void* data, image::Format pixelFormat, int sizeX, int sizeY, int pitch);
				
		nm::index2 psdOrigin; 
		CRectPlacement::TRect dstRect;
		CRectPlacement::TRect srcRect;

		image::ImageInfo externImage;

		bool hasAlphaChannel = false;
		std::string name;
		int order = -1;

		void optimize();
		void updateTexDstRect();
		void release();
	};

	~TextureAtlasComposer();
	
	void build(RectAtlas& outAtlas, AtlasImage& atlasImage);

	RectSource& addEmptySourceRect(const std::string& name, int order);
	RectSource& addSourceRect(const std::string& fileName, const AtlasImage& img);
	RectSource& addSourceRectMem(const std::string& name, int order, void* data, image::Format pixelFormat, int sizeX, int sizeY, int pitch);
	RectSource& addSourceRect(const std::string& name, const std::string& fileName);
	RectSource& addSourceRect(const std::string& fileName);

	template<class ImageOperation>
	RectSource& addSourceRectOp(const std::string& name, const std::string& fileName, ImageOperation&& op)
	{
		AtlasImage img;
		image::freeimage::load(fileName.c_str(), img);
		op(img);
		return addSourceRect(name, img);
	}

	template<class ImageOperation>
	RectSource& addSourceRectOp(const std::string& fileName, ImageOperation&& op)
	{
		return addSourceRectOp(fileName, fileName, std::forward<ImageOperation>(op));
	}

	void addSourceDir(const std::string& dirName, bool recursive, bool visibleLayers);
	void addSourcePSD(const std::string& fileName, bool visibleLayers);

	void addImagesSourceDir(const std::string& dirName, const std::string& extension, bool recursive);

	template<class ImageOperation>
	void addImagesSourceDir(const std::string& dirName, const std::string& extension, bool recursive, ImageOperation&& op)
	{
		auto files = Base::FileUtils::GetFileNamesByMask(dirName + "/", extension, true);
		std::sort(files.begin(), files.end());

		for (const auto& fileName : files)
		{
			addSourceRectOp(fileName.string(), std::forward<ImageOperation>(op));
		}
	}

	void clear();

private:
	void composeAtlas();
	void createAtlasTexture(RectAtlas& atlas, std::vector<RectSource*>& output, AtlasImage& atlasImage);
	void saveAtlas(const std::string& outFile, bool genMips);

	void preProcess(std::vector<RectSource*>& output);

	typedef std::map<std::string, RectSource> SourcesMap;
	SourcesMap sourceRects;

	RectAtlas::Size sourceMaxSize;
};

#endif



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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