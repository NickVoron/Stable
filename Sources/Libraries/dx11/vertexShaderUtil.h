#pragma once

namespace dx11
{
	template<class E> struct VSParam;
	template<> struct VSParam<Geometry::POS1> { static const char* text() { return "float  position : POSITION"; }; };
	template<> struct VSParam<Geometry::POS2> { static const char* text() { return "float2 position : POSITION"; }; };
	template<> struct VSParam<Geometry::POS3> { static const char* text() { return "float3 position : POSITION"; }; };
	template<> struct VSParam<Geometry::POS4> { static const char* text() { return "float4 position : POSITION"; }; };

	template<> struct VSParam<Geometry::NORMAL> { static const char* text() { return "float3 normal : NORMAL"; }; };
	template<> struct VSParam<Geometry::TANGENT> { static const char* text() { return "float3 tangent : TANGENT"; }; };

	template<> struct VSParam<Geometry::DIFFUSE>  { static const char* text() { return "float4 diffuse  : COLOR"; }; };
	template<> struct VSParam<Geometry::EMISSION> { static const char* text() { return "float4 emission : COLOR"; }; };
	template<> struct VSParam<Geometry::SPECULAR> { static const char* text() { return "float4 specular : COLOR"; }; };

	template<> struct VSParam<Geometry::BLENDWEIGHT<0>>		{ static const char* text() { return "float4 weights0 : BLENDWEIGHT0"; }; };
	template<> struct VSParam<Geometry::BLENDWEIGHT<1>>		{ static const char* text() { return "float4 weights1 : BLENDWEIGHT1"; }; };
	template<> struct VSParam<Geometry::BLENDWEIGHT<2>>		{ static const char* text() { return "float4 weights2 : BLENDWEIGHT2"; }; };
	template<> struct VSParam<Geometry::BLENDWEIGHT<3>>		{ static const char* text() { return "float4 weights3 : BLENDWEIGHT3"; }; };
	template<> struct VSParam<Geometry::BLENDINDICES<0>>	{ static const char* text() { return "uint4 indices0 : BLENDINDICES0"; }; };
	template<> struct VSParam<Geometry::BLENDINDICES<1>>	{ static const char* text() { return "uint4 indices1 : BLENDINDICES1"; }; };
	template<> struct VSParam<Geometry::BLENDINDICES<2>>	{ static const char* text() { return "uint4 indices2 : BLENDINDICES2"; }; };
	template<> struct VSParam<Geometry::BLENDINDICES<3>>	{ static const char* text() { return "uint4 indices3 : BLENDINDICES3"; }; };


	template<> struct VSParam<Geometry::UV1<0> > { static const char* text() { return "float  uv10 : TEXCOORD0"; }; };
	template<> struct VSParam<Geometry::UV2<0> > { static const char* text() { return "float2 uv20 : TEXCOORD0"; }; };
	template<> struct VSParam<Geometry::UV3<0> > { static const char* text() { return "float3 uv30 : TEXCOORD0"; }; };
	template<> struct VSParam<Geometry::UV4<0> > { static const char* text() { return "float4 uv40 : TEXCOORD0"; }; };

	template<> struct VSParam<Geometry::UV1<1> > { static const char* text() { return "float  uv11 : TEXCOORD1"; }; };
	template<> struct VSParam<Geometry::UV2<1> > { static const char* text() { return "float2 uv21 : TEXCOORD1"; }; };
	template<> struct VSParam<Geometry::UV3<1> > { static const char* text() { return "float3 uv31 : TEXCOORD1"; }; };
	template<> struct VSParam<Geometry::UV4<1> > { static const char* text() { return "float4 uv41 : TEXCOORD1"; }; };

	template<> struct VSParam<Geometry::UV1<2> > { static const char* text() { return "float  uv12 : TEXCOORD2"; }; };
	template<> struct VSParam<Geometry::UV2<2> > { static const char* text() { return "float2 uv22 : TEXCOORD2"; }; };
	template<> struct VSParam<Geometry::UV3<2> > { static const char* text() { return "float3 uv32 : TEXCOORD2"; }; };
	template<> struct VSParam<Geometry::UV4<2> > { static const char* text() { return "float4 uv42 : TEXCOORD2"; }; };

	template<> struct VSParam<Geometry::UV1<3> > { static const char* text() { return "float  uv13 : TEXCOORD3"; }; };
	template<> struct VSParam<Geometry::UV2<3> > { static const char* text() { return "float2 uv23 : TEXCOORD3"; }; };
	template<> struct VSParam<Geometry::UV3<3> > { static const char* text() { return "float3 uv33 : TEXCOORD3"; }; };
	template<> struct VSParam<Geometry::UV4<3> > { static const char* text() { return "float4 uv43 : TEXCOORD3"; }; };

	template<> struct VSParam<Geometry::UV1<4> > { static const char* text() { return "float  uv14 : TEXCOORD4"; }; };
	template<> struct VSParam<Geometry::UV2<4> > { static const char* text() { return "float2 uv24 : TEXCOORD4"; }; };
	template<> struct VSParam<Geometry::UV3<4> > { static const char* text() { return "float3 uv34 : TEXCOORD4"; }; };
	template<> struct VSParam<Geometry::UV4<4> > { static const char* text() { return "float4 uv44 : TEXCOORD4"; }; };

	template<> struct VSParam<Geometry::UV1<5> > { static const char* text() { return "float  uv15 : TEXCOORD5"; }; };
	template<> struct VSParam<Geometry::UV2<5> > { static const char* text() { return "float2 uv25 : TEXCOORD5"; }; };
	template<> struct VSParam<Geometry::UV3<5> > { static const char* text() { return "float3 uv35 : TEXCOORD5"; }; };
	template<> struct VSParam<Geometry::UV4<5> > { static const char* text() { return "float4 uv45 : TEXCOORD5"; }; };

	template<> struct VSParam<Geometry::UV1<6> > { static const char* text() { return "float  uv16 : TEXCOORD6"; }; };
	template<> struct VSParam<Geometry::UV2<6> > { static const char* text() { return "float2 uv26 : TEXCOORD6"; }; };
	template<> struct VSParam<Geometry::UV3<6> > { static const char* text() { return "float3 uv36 : TEXCOORD6"; }; };
	template<> struct VSParam<Geometry::UV4<6> > { static const char* text() { return "float4 uv46 : TEXCOORD6"; }; };

	template<> struct VSParam<Geometry::UV1<7> > { static const char* text() { return "float  uv17 : TEXCOORD7"; }; };
	template<> struct VSParam<Geometry::UV2<7> > { static const char* text() { return "float2 uv27 : TEXCOORD7"; }; };
	template<> struct VSParam<Geometry::UV3<7> > { static const char* text() { return "float3 uv37 : TEXCOORD7"; }; };
	template<> struct VSParam<Geometry::UV4<7> > { static const char* text() { return "float4 uv47 : TEXCOORD7"; }; };

	template<class VertexList>
	struct VertexToComponent
	{
		template<class InputList, class VL> struct impl;

		template<class InputList, class VL> struct impl
		{
			typedef typename Loki::TL::Append<InputList, typename VL::Head::ComponentsList>::Result Result;
		};

		template<class InputList> 
		struct impl<InputList, Loki::NullType>
		{
			typedef InputList Result;
		};

		typedef typename impl<typename VertexList::Head::ComponentsList, typename VertexList::Tail>::Result Result;		
	};

	template<class ComponentsList>
	struct VertexShaderInputData
	{
		template<class VL> struct impl;
		template<class VL> struct impl
		{
			static std::string text(const std::string& input) 
			{ 
				return impl<VL::Tail>::text( input + (!input.empty() ? ", " : "") + VSParam<VL::Head>::text() );
			}
		};

		template<> struct impl<Loki::NullType>
		{
			static std::string text(const std::string& input) { return input; }
		};

		static std::string text() { return impl<ComponentsList>::text(""); }
	};

	template<class VertexList>
	struct VertexShaderText
	{
		static std::string prologue()
		{
			return "float4 VS( ";
		}

		static std::string epilogue()
		{
			return " ) : SV_POSITION { float4 res = float4(0,0,0,0); return res; }";
		}

		static std::string text() 
		{ 
			typedef typename VertexToComponent<VertexList>::Result ComponentsList;
			return prologue() + VertexShaderInputData<ComponentsList>::text() + epilogue(); 
		}
	};

	struct VSMemBlk : public Base::IntrusiveList<VSMemBlk>::Node { mem::mem_desc memblock; };

	struct VertexShaderUtilStorage : public Base::IntrusiveList<VSMemBlk>
	{
		static void release()
		{
			for (auto& md : container())
			{
				mem::deallocate(md.memblock);
			}

			container().remove_all();
		}

		static VertexShaderUtilStorage& container()
		{
			static VertexShaderUtilStorage cont;
			return cont;
		}
	};


	template<class VertexList>
	struct VertexShaderUtil
	{
		static mem::mem_desc& code() 
		{
			static VSMemBlk memblock;

			if (!memblock.isInList())
			{
				VertexShaderUtilStorage::container().push_back(memblock);
				std::string txt = VertexShaderText<VertexList>::text();
				ShaderCompiler::compile(txt, "VS", "vs_5_0", memblock.memblock);
			}			
			return memblock.memblock;
		}
	};
}

