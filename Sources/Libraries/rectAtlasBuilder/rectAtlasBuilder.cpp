#include "rectAtlasBuilder.h"
#include "rectAtlas/library.include.h"
#include "libpsd/library.include.h"
#include "defaultLogs/library.include.h"


TextureAtlasComposer::RectSource::RectSource()
{
	order = -1;
	psdOrigin = nm::index2::zeroIndex;
}

void TextureAtlasComposer::RectSource::release()
{
	externImage.clear();
}

void TextureAtlasComposer::RectSource::addMemBlock(void* data, image::Format pixelFormat, int sizeX, int sizeY, int pitch)
{
	externImage.init(data, true, pixelFormat, sizeX, sizeY, pitch);
	
	unsigned int memSize = sizeY * pitch;
	externImage.data = malloc(memSize);
	mem::memset(externImage.data, 0, memSize);
	mem::memcpy(externImage.data, data, memSize);
	

	srcRect.pos.x = dstRect.pos.x = 0;
	srcRect.pos.y = dstRect.pos.y = 0;

	dstRect.size.x = sizeX;
	dstRect.size.y = sizeY;

	srcRect.size.x = sizeX;
	srcRect.size.y = sizeY;

	ENFORCE(externImage.valid());

	if (pixelFormat != image::R32F)
	{
		hasAlphaChannel = image::alpha_channel_used(externImage);
	}	
}

void TextureAtlasComposer::RectSource::optimize()
{
	int minX = srcRect.right();
	int minY = srcRect.bottom();
	int maxX = srcRect.left();
	int maxY = srcRect.top();

	typedef image::Color<image::ARGB8> Pixel;
	typedef image::Plane<Pixel> Image;
	Image img(externImage.data, externImage.pitchInBytes, externImage.sizeX, externImage.sizeY);

	for (int y = 0; y < srcRect.size.y; ++y)
	{
		bool found = false;
		for (int x = 0; x < srcRect.size.x; ++x)
		{
			Pixel& pixel = img(x, y);
			unsigned char a = pixel.a();
			if(a > 100)
			{
				minX = x;
				minY = y;
				found = true;
				break;
			}
		}

		if(found)
			break;
	}

	for (int y = srcRect.size.y - 1; y >= 0 ; --y)
	{
		bool found = false;
		for (int x = srcRect.size.x - 1; x >= 0; --x)
		{
			Pixel& pixel = img(x, y);
			if(pixel.a() > 100)
			{
				maxX = x;
				maxY = y;
				found = true;
				break;
			}
		}

		if(found)
			break;
	}
}

TextureAtlasComposer::~TextureAtlasComposer()
{
	clear();
}

void TextureAtlasComposer::build(RectAtlas& atlas, AtlasImage& atlasImage)
{
	std::vector<RectSource*> rsl;
	preProcess(rsl);

	CRectPlacement rectPlacement;
	for(RectSource* r : rsl)
	{
		//rectPlacement.AddAtEmptySpotAutoGrow (&r->dstRect, 8192, 8192);
		//ENFORCE( rectPlacement.AddAtEmptySpotAutoGrow (&r->dstRect, 8192, 8192) );
		ENFORCE( rectPlacement.AddAtEmptySpotAutoGrow (&r->dstRect, 16384, 16384) );
	}

//	LOG_MSG( "result atlas size:" << nm::index2(rectPlacement.GetW(), rectPlacement.GetH()) );

	//сортировка по z-order
	std::sort(rsl.begin(), rsl.end(), [](const RectSource* r0, const RectSource* r1) { return r0->order < r1->order; });

	int count = (int)rsl.size(); 
	atlas.init( count, rectPlacement.GetW(), rectPlacement.GetH() );
	atlas.sourceSz = sourceMaxSize;

	for (int i = 0; i < count; ++i)
	{
		CRectPlacement::TRect& r = rsl[i]->dstRect;

		atlas.rects[i].orgX = (float)rsl[i]->psdOrigin.x;
		atlas.rects[i].orgY = (float)rsl[i]->psdOrigin.y;
		atlas.rects[i].minX = (float)r.left();
		atlas.rects[i].minY = (float)r.top();
		atlas.rects[i].maxX = (float)r.right();
		atlas.rects[i].maxY = (float)r.bottom();

		atlas.namesTable[rsl[i]->name.c_str()] = i;
	}

	createAtlasTexture(atlas, rsl, atlasImage);
}

TextureAtlasComposer::RectSource& TextureAtlasComposer::addEmptySourceRect(const std::string& name, int order)
{
	TextureAtlasComposer::RectSource& newRect = sourceRects[name];
	newRect.name = name;
	newRect.order = order;
	return newRect;
}

TextureAtlasComposer::RectSource& TextureAtlasComposer::addSourceRect(const std::string& name, const std::string& fileName)
{	
	return addSourceRectOp(name, fileName, [](AtlasImage& img) {});
}	

TextureAtlasComposer::RectSource& TextureAtlasComposer::addSourceRect(const std::string& fileName)
{
	return addSourceRect(fileName, fileName);
}

TextureAtlasComposer::RectSource& TextureAtlasComposer::addSourceRect(const std::string& name, const AtlasImage& img)
{
	return addSourceRectMem(name, sourceRects.size(), img.bmp, image::ABGR32F, img.sizeX, img.sizeY, img.pitchInBytes);
}					  

TextureAtlasComposer::RectSource& TextureAtlasComposer::addSourceRectMem(const std::string& name, int order, void* data, image::Format pixelFormat, int sizeX, int sizeY, int pitch)
{
	RectSource& rect = addEmptySourceRect(name, order);
	if(data)
		rect.addMemBlock(data, pixelFormat, sizeX, sizeY, pitch);
	return rect;
}

void TextureAtlasComposer::addImagesSourceDir(const std::string& dirName, const std::string& extension, bool recursive)
{
	return addImagesSourceDir(dirName, extension, recursive, [](AtlasImage& img) {});
}

void TextureAtlasComposer::addSourceDir(const std::string& dirName, bool recursive, bool visibleLayers)
{
	auto files = Base::FileUtils::GetFileNamesByMask(dirName + "/", "psd", true);
	std::sort(files.begin(), files.end());

	for(const auto& fileName : files)
	{
		addSourcePSD(fileName.string(), visibleLayers);
	}
}

void TextureAtlasComposer::addSourcePSD(const std::string& fileName, bool visibleLayers)
{
	ENFORCE(sizeof(psd_argb_color) == image::pixel_info<image::ARGB8>::size);

	psd_context * context = NULL;
	psd_status status;

	// parse the psd file to psd_context
//	LOG_MSG("load psd file: " << fileName);
	status = psd_image_load(&context, (psd_char *)fileName.c_str());
	ENFORCE(status == psd_status_done);

	if(sourceMaxSize.x < context->width) sourceMaxSize.x = context->width;
	if(sourceMaxSize.y < context->height) sourceMaxSize.y = context->height;
	
	for (int i = 0; i < context->layer_count; ++i)
	{
		psd_layer_record& lr = context->layer_records[i];
		if (lr.layer_type == psd_layer_type_folder)
			continue;

		if(lr.width != 0 && lr.height != 0 && ((visibleLayers && lr.visible) || !visibleLayers))
		{
			RectSource& rect = addSourceRectMem((const char*)lr.layer_name, i, lr.image_data, image::ARGB8, lr.width, lr.height, sizeof(psd_argb_color) * lr.width);
			rect.psdOrigin.x = lr.left;
			rect.psdOrigin.y = lr.top;
		}
	}

	// free if it's done
	psd_image_free(context);
	ENFORCE(status == psd_status_done);
}

void TextureAtlasComposer::preProcess(std::vector<RectSource*>& output)
{
	output.reserve( sourceRects.size() );
	for(SourcesMap::value_type& v : sourceRects)
	{
		RectSource* sourceRect = &v.second;
		sourceRect->optimize();

		output.push_back(sourceRect);
	}

	//сортировка по размеру для эффективной упаковки
	std::sort(output.begin(), output.end(), [](const RectSource* r0, const RectSource* r1) {return ::greater(r0->srcRect, r1->srcRect); });
}

void TextureAtlasComposer::createAtlasTexture(RectAtlas& atlas, std::vector<RectSource*>& output, AtlasImage& img)
{
	RectAtlas::Size s = atlas.size();
	img.allocate(s.x, s.y);

	int count = (int)output.size();
	for(int i = 0; i < count; ++i)
	{
		RectSource* rs = output[i];
		RectAtlas::Rect& rect = atlas.absoluteRect(i);
		RectAtlas::Bitmask& bitmask = atlas.bitmask(i);

		const CRectPlacement::TRect& sr = rs->srcRect;
		const CRectPlacement::TRect& dr = rs->dstRect;

		const image::ImageInfo& externImage = rs->externImage;
		image::ImageInfo sii(image::ABGR32F, externImage.sizeX, externImage.sizeY);
		image::convert_plane_image(externImage, sii);
		AtlasImage src(sii.data, sii.pitchInBytes, sii.sizeX, sii.sizeY);
		image::copy_rect(src, img, sr, dr);
		image::flip_vertical(src, src);

		AtlasAlpha alpha;
		alpha.allocate(src.sizeX, src.sizeY);
		image::copy_a_to_r(src, alpha);

		bitmask.create((char*)alpha.bmp, alpha.sizeX, alpha.sizeY);

		alpha.clear();
	}


	AtlasImage simg;
	simg.copy(img);
	image::flip_vertical(simg, simg);
}

void TextureAtlasComposer::clear()
{
	for(SourcesMap::value_type& v : sourceRects)
	{
		v.second.release();
	}

	sourceRects.clear();
}

