#pragma once

#include "rectAtlas/library.include.h"
#include "image/library.include.h"
#include "imageD3D/library.include.h"
#include "stream/library.include.h"

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
				
		nm::index2 psdOrigin; // позиция слоя в psd файле
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