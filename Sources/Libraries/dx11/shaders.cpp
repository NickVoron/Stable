// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "shaders.h"
#include <d3dcompiler.h>
#include <D3DX11.h>

#include "resourceUtils/library.include.h"

namespace dx11
{
	class ShaderInclude : public ID3DInclude
	{
	public:
		ShaderInclude(const WCHAR* szFileName) : parentFileName(szFileName){}

		HRESULT __stdcall Open(	D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		HRESULT __stdcall Close(LPCVOID pData);

		std::wstring parentFileName;		
 	};
	
	HRESULT __stdcall ShaderInclude::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR fileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
	{
		try
		{	
			
			

			boost::filesystem::path fp = Base::StrUtils::Convert(parentFileName);
			fp.normalize();
			
			boost::filesystem::path finalPath;
			switch (IncludeType)
			{
			case D3D_INCLUDE_LOCAL: 
			{
				finalPath = fp.remove_filename();
				finalPath /= fileName;
				break;
			}
			case D3D_INCLUDE_SYSTEM:
			{
				finalPath = fileName; break; 
			}
			default: THROW("unknown include type");
			}



			

			std::ifstream fileStream;
			fileStream.open(finalPath.c_str(), std::ios::binary);
			
			int sz = Base::FileUtils::GetFileSize(finalPath.string());
			
			if (sz)
			{
				char* buf = new char[sz];
				fileStream.read(buf, sz);

				*ppData = buf;
				*pBytes = sz;
			}
			else
			{
				*ppData = nullptr;
				*pBytes = 0;
			}

			return S_OK;
		}
		catch (std::exception& err)
		{
			LOG_ERROR(err);
			return E_FAIL;
		}
	}

	HRESULT __stdcall ShaderInclude::Close(LPCVOID pData)
	{
		
		char* buf = (char*) pData;
		delete[] buf;
		return S_OK;
	}


	HRESULT shaderCompilationErrorHandle(HRESULT hr, ID3DBlob* pBlobOut, ID3DBlob* pErrorBlob, mem::mem_desc& memblock)
	{
		if( FAILED(hr) )
		{
			if( pErrorBlob != NULL )
			{
				OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
				LOG_ERROR ((char*)pErrorBlob->GetBufferPointer());
			}
			if( pErrorBlob ) pErrorBlob->Release();
			return hr;
		}

		if( pErrorBlob ) 
		{
			pErrorBlob->Release();
		}

		if(SUCCEEDED(hr))
		{
			mem::allocate(memblock, pBlobOut->GetBufferSize());
			mem::memcpy(memblock.data, pBlobOut->GetBufferPointer(), pBlobOut->GetBufferSize());
			pBlobOut->Release();
		}

		return hr;
	}

	DWORD shaderFlags()
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined( DEBUG ) || defined( _DEBUG )
		
		
		
		
		dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		return dwShaderFlags;
	}

	

	HRESULT compileShader( const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock)
	{
		ShaderInclude includeProcessor(szFileName);
		ID3DBlob* pBlobOut;
		ID3DBlob* pErrorBlob;
		HRESULT hr = D3DX11CompileFromFile(szFileName, NULL, &includeProcessor, szEntryPoint, szShaderModel, shaderFlags(), 0, NULL, &pBlobOut, &pErrorBlob, NULL);
		

		return shaderCompilationErrorHandle(hr, pBlobOut, pErrorBlob, memblock);
	}

	HRESULT compileShader( LPCSTR shaderText, unsigned int shaderLen, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock)
	{
		ID3DBlob* pBlobOut;
		ID3DBlob* pErrorBlob;
		HRESULT hr = D3DX11CompileFromMemory( shaderText, shaderLen, 0, NULL, NULL, szEntryPoint, szShaderModel, shaderFlags(), 0, NULL, &pBlobOut, &pErrorBlob, NULL );
		return shaderCompilationErrorHandle(hr, pBlobOut, pErrorBlob, memblock);
	}


	bool ShaderCompiler::compile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock)
	{
		return SUCCEEDED( compileShader(szFileName, szEntryPoint, szShaderModel, memblock) );
	}

	bool ShaderCompiler::compile(const std::string& shaderText, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock)
	{
		return SUCCEEDED( compileShader(shaderText.c_str(), shaderText.size(), szEntryPoint, szShaderModel, memblock) );
	}

	
	
	
	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::createImpl() 
	{
		device->device->CreateGeometryShaderWithStreamOutput(memblock.data, memblock.len, data.entries, data.numEntries, data.strides, data.numStrides, data.rasterizedStream, NULL, &ptr);
	} 
	
	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::save(stream::ostream& os)  const	
	{ 
		data.save(os);
		Parent::save(os);		
	}

	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::load(stream::istream& is)
	{ 
		data.load(is);
		Parent::load(is);
	}

	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::bind(const Data& d)
	{
		data = d;
	}

	ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::Data()
	{
		numEntries = 0;
		numStrides = 0;
		rasterizedStream = D3D11_SO_NO_RASTERIZED_STREAM;
	}

	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::save(stream::ostream& os) const
	{
		os << numEntries << numStrides << rasterizedStream;

		if(numStrides)
		{
			os.write(strides, sizeof(strides[0])*numStrides);
		}			

		for (unsigned int i = 0; i < numEntries; ++i)
		{
			saveEntry(os, entries[i]);
		}
	}

	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::load(stream::istream& is)
	{
		is >> numEntries >> numStrides >> rasterizedStream;

		if(numStrides)
		{
			is.read(strides, sizeof(strides[0])*numStrides);
		}				

		for (unsigned int i = 0; i < numEntries; ++i)
		{
			loadEntry(is, entries[i]);
		}
	}

	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::saveEntry(stream::ostream& os, const D3D11_SO_DECLARATION_ENTRY& entry)
	{
		os << entry.Stream << entry.SemanticIndex << entry.StartComponent << entry.ComponentCount << entry.OutputSlot << semanticIndex(entry.SemanticName);
	}

	void ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::loadEntry(stream::istream& is, D3D11_SO_DECLARATION_ENTRY& entry)
	{
		int semanticIdx;
		is >> entry.Stream >> entry.SemanticIndex >> entry.StartComponent >> entry.ComponentCount >> entry.OutputSlot >> semanticIdx;
		entry.SemanticName = semanticName(semanticIdx);
	}

	const char* ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::semanticName(int idx)
	{
		static const char* semantics[1] = {"POSITION"};

		return semantics[idx];
	}

	int ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER>::Data::semanticIndex(const char* name)
	{
		return 0;
	}

}








// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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