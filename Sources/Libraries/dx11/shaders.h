// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <D3D11.h>

#include "memory/library.include.h"
#include "stream/library.include.h"
#include "gapi/library.include.h"

#include "interfaces.h"
#include "constantBuffer.h"
#include "vertexShaderUtil.h"
#include "streamOutDeclaration.h"
#include "interfaces.h"


namespace dx11
{
	
	
	
	
	HRESULT compileShader( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock);
	HRESULT compileShader( LPCSTR shaderText, unsigned int shaderLen, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock);

	inline void shader_set(ID3D11DeviceContext* context, ID3D11VertexShader* ptr)		{ context->VSSetShader(ptr, 0, 0); }
	inline void shader_set(ID3D11DeviceContext* context, ID3D11PixelShader* ptr)		{ context->PSSetShader(ptr, 0, 0); }
	inline void shader_set(ID3D11DeviceContext* context, ID3D11GeometryShader* ptr)		{ context->GSSetShader(ptr, 0, 0); }
	inline void shader_set(ID3D11DeviceContext* context, ID3D11DomainShader* ptr)		{ context->DSSetShader(ptr, 0, 0); }
	inline void shader_set(ID3D11DeviceContext* context, ID3D11HullShader* ptr)			{ context->HSSetShader(ptr, 0, 0); }
	inline void shader_set(ID3D11DeviceContext* context, ID3D11ComputeShader* ptr)		{ context->CSSetShader(ptr, 0, 0); }

	
	template<gapi::ShaderTypeName shaderType>  struct DeviceShader;

	template<class ShaderInterfaceType, gapi::ShaderTypeName shaderType>
	struct ShaderDefBase : public DeviceResource<ShaderInterfaceType, DeviceShader<shaderType> >
	{
		typedef ShaderInterfaceType	D3DShaderType;
		typedef ShaderDefBase		Parent;

		virtual ~ShaderDefBase() { mem::deallocate(memblock); }

		void create(const mem::mem_desc& memblk)
		{
			memblock.copy(memblk);
			createImpl();
		}

		virtual void createImpl() = 0;

		virtual void save(stream::ostream& os)  const	{ os << memblock; }
		virtual void load(stream::istream& is)			{ is >> memblock; createImpl(); }

		void set() const 
		{ 
			shader_set(this->device->context, this->ptr);
			setted() = (this->ptr != 0);
		}

		static void reset()		
		{ 
			if (setted()) 
			{ 
				for (int i = 0; i < devices->count(); ++i)	
				{ 
					shader_set((*devices)[i].context, (ShaderInterfaceType*) 0);
				} 
				setted() = false; 
			} 
		}

		static bool& setted() { static bool value = false; return value; }

		mem::mem_desc memblock;
	};

	template<gapi::ShaderTypeName shaderType> struct ShaderDef;
	template<gapi::ShaderTypeName shaderType> struct ShaderBase;

	template<> struct ShaderDef<gapi::VERTEX_SHADER>				: public ShaderDefBase<ID3D11VertexShader,		gapi::VERTEX_SHADER>{};
	template<> struct ShaderDef<gapi::PIXEL_SHADER>					: public ShaderDefBase<ID3D11PixelShader,		gapi::PIXEL_SHADER>{};
	template<> struct ShaderDef<gapi::GEOMETRY_SHADER>				: public ShaderDefBase<ID3D11GeometryShader,	gapi::GEOMETRY_SHADER>{};
	template<> struct ShaderDef<gapi::DOMAIN_SHADER>				: public ShaderDefBase<ID3D11DomainShader,		gapi::DOMAIN_SHADER>{};
	template<> struct ShaderDef<gapi::HULL_SHADER>					: public ShaderDefBase<ID3D11HullShader,		gapi::HULL_SHADER>{};
	template<> struct ShaderDef<gapi::COMPUTE_SHADER>				: public ShaderDefBase<ID3D11ComputeShader,		gapi::COMPUTE_SHADER>{};
	template<> struct ShaderDef<gapi::GEOMETRY_STREAMOUT_SHADER>	: public ShaderDefBase<ID3D11GeometryShader,	gapi::GEOMETRY_STREAMOUT_SHADER>{};


	
	
	
	template<> 
	struct ShaderBase<gapi::VERTEX_SHADER> : public ShaderDef<gapi::VERTEX_SHADER>
	{
		virtual void createImpl()	{ device->device->CreateVertexShader(memblock.data, memblock.len, NULL, &ptr); }
		static const char* EntryPointName() { return "VS"; }
		static const char* ShaderModelName() { return "vs_5_0"; }
	};

	
	template<> 
	struct ShaderBase<gapi::PIXEL_SHADER> : public ShaderDef<gapi::PIXEL_SHADER>
	{
		virtual void createImpl() { device->device->CreatePixelShader(memblock.data, memblock.len, NULL, &ptr); }
		static const char* EntryPointName() { return "PS"; }
		static const char* ShaderModelName() { return "ps_5_0"; }
	};

	
	template<> 
	struct ShaderBase<gapi::GEOMETRY_SHADER> : public ShaderDef<gapi::GEOMETRY_SHADER>
	{
		virtual void createImpl() { device->device->CreateGeometryShader(memblock.data, memblock.len, NULL, &ptr); }
		static const char* EntryPointName() { return "GS"; }
		static const char* ShaderModelName() { return "gs_5_0"; }
	};

	
	template<> 
	struct ShaderBase<gapi::GEOMETRY_STREAMOUT_SHADER> : public ShaderDef<gapi::GEOMETRY_STREAMOUT_SHADER>
	{
		struct Data
		{
			Data();

			void save(stream::ostream& os) const;
			void load(stream::istream& is);

			static void saveEntry(stream::ostream& os, const D3D11_SO_DECLARATION_ENTRY& entry);
			static void loadEntry(stream::istream& is, D3D11_SO_DECLARATION_ENTRY& entry);

			static const char* semanticName(int idx);
			static int semanticIndex(const char* name);

			template<class VertexType>
			void init()
			{
				strides[numStrides++] = sizeof(VertexType);

				StreamOutElementsArray<Loki::MakeTypelist<VertexType>>::init(entries);
				numEntries = StreamOutElementsArray<Loki::MakeTypelist<VertexType>>::elementsCount;
			}

			D3D11_SO_DECLARATION_ENTRY entries[D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT];
			unsigned int strides[D3D11_SO_BUFFER_SLOT_COUNT];
			unsigned int numEntries;
			unsigned int numStrides;
			unsigned int rasterizedStream;
		};

		static const char* EntryPointName() { return "GS"; }
		static const char* ShaderModelName() { return "gs_5_0"; }

		virtual void createImpl();
		virtual void save(stream::ostream& os) const;	
		virtual void load(stream::istream& is);

		void bind(const Data& d);

		Data data;
	};

	
	template<> 
	struct ShaderBase<gapi::DOMAIN_SHADER> : public ShaderDef<gapi::DOMAIN_SHADER>
	{
		virtual void createImpl()	{ device->device->CreateDomainShader(memblock.data, memblock.len, NULL, &ptr); }
		static const char* EntryPointName() { return "DS"; }
		static const char* ShaderModelName() { return "ds_5_0"; }
	};

	
	template<> 
	struct ShaderBase<gapi::HULL_SHADER> : public ShaderDef<gapi::HULL_SHADER>
	{
		virtual void createImpl() { device->device->CreateHullShader(memblock.data, memblock.len, NULL, &ptr); }
		static const char* EntryPointName() { return "HS"; }
		static const char* ShaderModelName() { return "hs_5_0"; }
	};

	
	template<> 
	struct ShaderBase<gapi::COMPUTE_SHADER> : public ShaderDef<gapi::COMPUTE_SHADER>
	{
		virtual void createImpl() { device->device->CreateComputeShader(memblock.data, memblock.len, NULL, &ptr); }
		static const char* EntryPointName() { return "CS"; }
		static const char* ShaderModelName() { return "cs_5_0"; }
	};
	
	
	template<gapi::ShaderTypeName shaderType>
	struct DeviceShader : public ShaderBase<shaderType>
	{
	};

	template<gapi::ShaderTypeName shaderType>
	struct Shader : public DeviceShader<shaderType>::StateSet
	{
		void save(stream::ostream& os)  const { for (int i = 0; i < devices->count(); ++i) { this->data[i].save(os); }	}
		void load(stream::istream& is)		  { for (int i = 0; i < devices->count(); ++i) { this->data[i].load(is); } }

		static const char* EntryPointName()		{ return ShaderBase<shaderType>::EntryPointName(); }
		static const char* ShaderModelName()	{ return ShaderBase<shaderType>::ShaderModelName(); }
	};

	
	
	
	struct ShaderCompiler 
	{
		template<gapi::ShaderTypeName shaderType>
		static bool create(Shader<shaderType>& shader, const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel)
		{
			mem::mem_desc memblock;
			if(compile(szFileName, szEntryPoint, szShaderModel, memblock))
			{
				shader.create(memblock);
				return true;
			}		

			return false;
		}

		template<gapi::ShaderTypeName shaderType>
		static bool create(Shader<shaderType>& shader, const WCHAR* szFileName)
		{
			return create(shader, szFileName, Shader<shaderType>::EntryPointName(), Shader<shaderType>::ShaderModelName());
		}

		template<gapi::ShaderTypeName shaderType>
		static bool create(Shader<shaderType>& shader, const std::string& shaderText, LPCSTR szEntryPoint, LPCSTR szShaderModel)
		{
			mem::mem_desc memblock;
			if(compile(shaderText, szEntryPoint, szShaderModel, memblock))
			{
				shader.create(memblock);
				return true;
			}	

			return false;
		}

		static bool compile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock);
		static bool compile(const std::string& shaderText, LPCSTR szEntryPoint, LPCSTR szShaderModel, mem::mem_desc& memblock);
	}; 



	
	
	
	struct ShaderSetter		{ template<gapi::ShaderTypeName shaderType>	void operator()(const Shader<shaderType>& shader, int deviceIndex)	{ shader.set(deviceIndex); } };
	struct ShaderDestroyer	{ template<gapi::ShaderTypeName shaderType> void operator()(Shader<shaderType>& shader)			{  }};

	struct ShaderSaver		{ ShaderSaver(stream::ostream& os_) :os(os_){}	template<gapi::ShaderTypeName shaderType> void operator()(const Shader<shaderType>& shader) { shader.save(os); }	stream::ostream& os; };
	struct ShaderLoader		{ ShaderLoader(stream::istream& is_):is(is_){}	template<gapi::ShaderTypeName shaderType> void operator()(Shader<shaderType>& shader)		{ shader.load(is); }	stream::istream& is; };

	struct ShaderComp
	{
		ShaderComp(const wchar_t* fn, bool& res):fileName(fn), succeded(res){}

		template<gapi::ShaderTypeName shaderType> void operator()(Shader<shaderType>& shader)
		{ 
			succeded |= ShaderCompiler::create(shader, fileName);			
		}
		const wchar_t* fileName;
		bool& succeded;
	};



	template<int shaders> struct ShaderSetStorage;
	template<>	struct ShaderSetStorage<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER>														 { typedef Loki::TypeTuple< Loki::MakeTypelist<Shader<gapi::VERTEX_SHADER>, Shader<gapi::PIXEL_SHADER>> > Storage; };
	template<>	struct ShaderSetStorage<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER>				 { typedef Loki::TypeTuple< Loki::MakeTypelist<Shader<gapi::VERTEX_SHADER>, Shader<gapi::PIXEL_SHADER>, Shader<gapi::HULL_SHADER>, Shader<gapi::DOMAIN_SHADER>> > Storage; };
	template<>	struct ShaderSetStorage<gapi::VERTEX_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER | gapi::GEOMETRY_STREAMOUT_SHADER> { typedef Loki::TypeTuple< Loki::MakeTypelist<Shader<gapi::VERTEX_SHADER>, Shader<gapi::HULL_SHADER>, Shader<gapi::DOMAIN_SHADER>, Shader<gapi::GEOMETRY_STREAMOUT_SHADER>> > Storage; };
	
	
	
	
	template<int shaders> struct ShadersResetter;
	template<>	struct ShadersResetter<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER>											
	{
		static void reset() 
		{
			DeviceShader<gapi::COMPUTE_SHADER>::reset();
			DeviceShader<gapi::HULL_SHADER>::reset();
			DeviceShader<gapi::DOMAIN_SHADER>::reset();
			DeviceShader<gapi::GEOMETRY_SHADER>::reset();
		}
	};

	template<> struct ShadersResetter<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER>	
	{
		static void reset() 
		{
			DeviceShader<gapi::COMPUTE_SHADER>::reset();
			DeviceShader<gapi::GEOMETRY_SHADER>::reset();
		}
	};

	template<> struct ShadersResetter<gapi::VERTEX_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER | gapi::GEOMETRY_STREAMOUT_SHADER>	
	{
		static void reset()
		{
			DeviceShader<gapi::COMPUTE_SHADER>::reset();
			DeviceShader<gapi::PIXEL_SHADER>::reset();
		}
	};

	
	
	
	template<int shaders>
	struct ShaderSetBase : public DrawableObject
	{
		typename ShaderSetStorage<shaders>::Storage storage;

		void clear()													{ Loki::TupleFunc::exec(storage, ShaderDestroyer());	}
		void save(stream::ostream& os) const							{ Loki::TupleFunc::exec(storage, ShaderSaver(os));		}
		void load(stream::istream& is)									{ Loki::TupleFunc::exec(storage, ShaderLoader(is));		}

		void set(int deviceIndex) const												
		{ 
			ShadersResetter<shaders>::reset(); 
			Loki::TupleFunc::exec(storage, ShaderSetter(), deviceIndex);
		}

		virtual void execute(int deviceIndex)
		{
			set(deviceIndex);
		}

		virtual bool compile(const wchar_t* fileName, const void* userData)
		{ 
			bool succeded = false;
			ShaderComp compiler(fileName, succeded);
			Loki::TupleFunc::exec(storage, compiler);
			
			return succeded;
		} 
	};

	template<int shaders> struct ShaderSet;

	template<>
	struct ShaderSet<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER> : public ShaderSetBase<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER>
	{
		Shader<gapi::VERTEX_SHADER>&	vertex()	{ return storage.get<Shader<gapi::VERTEX_SHADER>>();	}
		Shader<gapi::PIXEL_SHADER>&		pixel()		{ return storage.get<Shader<gapi::PIXEL_SHADER>>();		}
	};

	template<>
	struct ShaderSet<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER> : public ShaderSetBase<gapi::VERTEX_SHADER | gapi::PIXEL_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER>
	{
		Shader<gapi::VERTEX_SHADER>&	vertex()	{ return storage.get<Shader<gapi::VERTEX_SHADER>>();	}
		Shader<gapi::PIXEL_SHADER>&		pixel()		{ return storage.get<Shader<gapi::PIXEL_SHADER>>();		}
		Shader<gapi::HULL_SHADER>&		hull()		{ return storage.get<Shader<gapi::HULL_SHADER>>();		}
		Shader<gapi::DOMAIN_SHADER>&	domain()	{ return storage.get<Shader<gapi::DOMAIN_SHADER>>();	}
	};

	template<>
	struct ShaderSet<gapi::VERTEX_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER | gapi::GEOMETRY_STREAMOUT_SHADER> : public ShaderSetBase<gapi::VERTEX_SHADER | gapi::HULL_SHADER | gapi::DOMAIN_SHADER | gapi::GEOMETRY_STREAMOUT_SHADER>
	{
		Shader<gapi::VERTEX_SHADER>&				vertex()	{ return storage.get<Shader<gapi::VERTEX_SHADER>>();			}
		Shader<gapi::HULL_SHADER>&					hull()		{ return storage.get<Shader<gapi::HULL_SHADER>>();				}
		Shader<gapi::DOMAIN_SHADER>&				domain()	{ return storage.get<Shader<gapi::DOMAIN_SHADER>>();			}
		Shader<gapi::GEOMETRY_STREAMOUT_SHADER>&	streamout()	{ return storage.get<Shader<gapi::GEOMETRY_STREAMOUT_SHADER>>();}

		virtual bool compile(const wchar_t* fileName, const void* userData)
		{ 
			DeviceShader<gapi::GEOMETRY_STREAMOUT_SHADER>::Data* data = (DeviceShader<gapi::GEOMETRY_STREAMOUT_SHADER>::Data*)userData;





			
			bool succeded = false;
			ShaderComp compiler(fileName, succeded);
			Loki::TupleFunc::exec(storage, compiler);

			return succeded;
		}
	};


	
	
	
	
	template<class VertexType>
	DeviceShader<gapi::GEOMETRY_STREAMOUT_SHADER>::Data* StreamOutData()
	{
		static DeviceShader<gapi::GEOMETRY_STREAMOUT_SHADER>::Data data;
		static bool inited = false;
		if(!inited) 
		{
			data.init<VertexType>();
			inited = true;
		}

		return &data;
	}
}








// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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