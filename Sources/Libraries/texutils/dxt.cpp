#include "dxt.h"

// #include "nvimage/Image.h"
// #include "nvimage/DirectDrawSurface.h"

namespace texutils {
namespace DXT {

/*
	struct MyOutputHandler : public nvtt::OutputHandler
	{
		MyOutputHandler(const char * name) : s(name) {}
		virtual ~MyOutputHandler() { }

		virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel)
		{
			// ignore.
		}

		// Output data.
		virtual bool writeData(const void * data, int size)
		{
			s.write(data, size);
			return true;
		}

		stream::ofstream<> s;
	};

	struct MyErrorHandler : public nvtt::ErrorHandler
	{
		virtual void error(nvtt::Error e)
		{
			//nvDebugBreak();
		}
	};*/

	// Set options for normal maps.
	/*	void setNormalMap(nvtt::InputOptions & inputOptions)
	{
		INCOMPLETE;

		inputOptions.setNormalMap(true);
		inputOptions.setConvertToNormalMap(false);
		inputOptions.setGamma(1.0f, 1.0f);
		inputOptions.setNormalizeMipmaps(true);
	}

	// Set options for color maps.
	void setColorMap(nvtt::InputOptions & inputOptions)
	{
		INCOMPLETE;
		inputOptions.setNormalMap(false);
		inputOptions.setConvertToNormalMap(false);
		inputOptions.setGamma(2.2f, 2.2f);
		inputOptions.setNormalizeMipmaps(false);
	}
*/
	void ConvertToDXT(const std::string& fileName, stream::ostream& s, bool isNormal, bool dxt5)
	{
		INCOMPLETE;
// 		using namespace dx9::Utils;
// 		using namespace Base::StrUtils;
// 
// 		static std::string tmpName = "out/tmp.dds";
// 
// 		if(!fileName.empty())	
// 		{
// 			Compress(fileName, tmpName, isNormal, dxt5, true);
// 			SaveTextureToStream(s, tmpName);	
// 		}
	}

	void Compress(const std::string& inputFileName, const std::string& outputFileName, bool isNormalMap, bool dxt5, bool genMips)
	{
		INCOMPLETE;

/*
		nvtt::InputOptions inputOptions;
		nv::DirectDrawSurface dds(inputFileName.c_str());

		inputOptions.setTextureLayout(nvtt::TextureType_2D, dds.width(), dds.height());

		unsigned int mipmapCount = dds.mipmapCount();
		unsigned int faceCount = 1;

		nv::Image mipmap;

		for (unsigned int f = 0; f < faceCount; f++)
		{
			for (unsigned int m = 0; m < mipmapCount; m++)
			{
				dds.mipmap(&mipmap, f, m);

				inputOptions.setMipmapData(mipmap.pixels(), mipmap.width(), mipmap.height(), 1, f, m);
			}
		}

		inputOptions.setMipmapGeneration(genMips);
		inputOptions.setWrapMode(nvtt::WrapMode_Clamp);

		isNormalMap ? setNormalMap(inputOptions) : setColorMap(inputOptions);

		nvtt::Format format = dxt5 ? nvtt::Format_BC3 : nvtt::Format_BC1;
		nvtt::CompressionOptions compressionOptions;
		compressionOptions.setFormat(format);
		compressionOptions.setQuality(nvtt::Quality_Production);
		compressionOptions.setColorWeights(1, 1, 0);

		nvtt::Compressor compressor;

		MyErrorHandler errorHandler;
		MyOutputHandler outputHandler(outputFileName.c_str());

		nvtt::OutputOptions outputOptions;
		outputOptions.setOutputHandler(&outputHandler);
		outputOptions.setErrorHandler(&errorHandler);

		compressor.process(inputOptions, compressionOptions, outputOptions);*/
	}

	void Compress(const char* inputFileName, stream::ostream& os, bool isNormalMap, bool dxt5, bool genMips)
	{
		INCOMPLETE;
		/*		ENFORCE(0 && "need implementation");

		if(Base::FileUtils::FileExists(inputFileName))
		{
			static std::string tmpName = "tmp.dds";

			LPDIRECT3DTEXTURE9 tex;
			tex = dx9::Utils::LoadTexture(inputFileName, false, true);
			dx9::Utils::SaveTextureDDS(inputFileName, tex);
			SAFE_RELEASE(tex);
			Compress(inputFileName, tmpName, isNormalMap, dxt5, genMips);
			dx9::Utils::SaveTextureToStream(os, tmpName);
		}*/
	}

} //
} //