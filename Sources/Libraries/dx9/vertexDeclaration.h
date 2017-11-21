#pragma once

#include <d3d9.h>

#include "device.h"

#include "geometry/library.include.h"
#include "containers/library.include.h"

#include "dx9/interfaces.h"

namespace dx9
{					  
	using namespace Geometry;

	template<class VertexList>
	struct ElementsArray
	{
		static const int elementsCount = VertexListElementsCounter<VertexList>::count;
		D3DVERTEXELEMENT9 dwDecl[elementsCount + 1];

		ElementsArray()	{	fillD3DVertexListElementsArray<0, VertexList>::init(dwDecl);	}
	};

	template<class VertexList>
	LPDIRECT3DVERTEXDECLARATION9 createVertexDeclaration()
	{
		ElementsArray<VertexList> elements;
		LPDIRECT3DVERTEXDECLARATION9 decl;
		device->CreateVertexDeclaration( elements.dwDecl, &decl );
		return decl;
	}   

	// переводим из нашего типа в D3DVERTEXELEMENT9 
	template<class VertexElement>
	struct VertexElementD3DMapping;

	template<class Vertex, int streamIdx, int totalCount, int i> 
	struct fillD3DVertexElementsArray;

	template<class Vertex, int streamIdx, int totalCount> 
	struct fillD3DVertexElementsArray<Vertex, streamIdx, totalCount, 0>
	{
		inline static void init(D3DVERTEXELEMENT9* arr, int offset)
		{
			arr[totalCount] = D3DDECL_END();
		}
	};

	template<class Vertex, int streamIdx, int totalCount, int i> 
	struct fillD3DVertexElementsArray
	{
		inline static void init(D3DVERTEXELEMENT9* arr, int offset)
		{
			typedef Loki::FieldGetter<Vertex::VertexTuple, totalCount - i>::ResultType ElementType;
			typedef VertexElementD3DMapping<ElementType> D3DMapping;
			
			D3DVERTEXELEMENT9& element = arr[totalCount - i];
			element.Stream		= streamIdx;
			element.Method		= D3DMapping::D3DDECLMETHOD;
			element.Type		= D3DMapping::D3DDECLTYPE;
			element.Usage		= D3DMapping::D3DDECLUSAGE;
			element.UsageIndex	= D3DMapping::USAGE_INDEX;
			element.Offset		= offset;

			fillD3DVertexElementsArray<Vertex, streamIdx, totalCount, i - 1>::init(arr, offset + D3DMapping::ADDITIONAL_OFFSET);
		}
	};

	template<int streamIdx, class VertexList> 
	struct fillD3DVertexListElementsArray;

	template<int streamIdx> 
	struct fillD3DVertexListElementsArray<streamIdx, Loki::NullType>
	{
		static inline void init(D3DVERTEXELEMENT9* arr)
		{
			*arr = D3DDECL_END();
		}
	};

	template<int streamIdx, class VertexList> 
	struct fillD3DVertexListElementsArray
	{
		typedef typename VertexList::Head Vertex;
		typedef typename VertexList::Tail Tail;
		static inline void init(D3DVERTEXELEMENT9* arr)
		{
			static const int elementsCount = Vertex::ELEMENTS_COUNT;
			fillD3DVertexElementsArray<Vertex, streamIdx, elementsCount, elementsCount>::init(arr, 0);
			fillD3DVertexListElementsArray<streamIdx + 1, Tail>::init(arr + elementsCount);
		}
	};

	//VertexDeclaration
	class VertexDeclaration
	{
	public:
		VertexDeclaration():vd(0){}
		~VertexDeclaration(){}

		void set() const
		{
			static LPDIRECT3DVERTEXDECLARATION9 currentDecl = 0;
			//if(currentDecl != *vd)
			{
				device->SetVertexDeclaration(*vd);
				currentDecl = *vd;
				incrementCounter();
			}
		}

		template<class StreamsVerticesList>
		void create()
		{
			static LPDIRECT3DVERTEXDECLARATION9 staticDecl = 0;
			if(!staticDecl)
			{
				staticDecl = createVertexDeclaration<StreamsVerticesList>();
				declarations.addEntry(&staticDecl);
			}
			
			vd = &staticDecl;
		}

		static void releaseAllDeclarations()
		{
			auto count = declarations.size();
			for (std::size_t i = 0; i < count; ++i)
			{
				SAFE_RELEASE(*declarations[i]);
			}
			declarations.clear();
		}

		LPDIRECT3DVERTEXDECLARATION9 getVD() const {return *vd;}

		static int changeCounter;
		static inline void resetCounter(){changeCounter = 0;}
		static inline void incrementCounter() {++changeCounter;}

	private:
		LPDIRECT3DVERTEXDECLARATION9* vd;
		static Base::FixedArray<LPDIRECT3DVERTEXDECLARATION9*, 1024> declarations; 
	};

	//
	// специализации
	//
	template<>
	struct VertexElementD3DMapping< POS1 >
	{
		static const int D3DDECLTYPE		= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD		= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE		= D3DDECLUSAGE_POSITION;
		static const int USAGE_INDEX		= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(POS1);
	};

	template<>
	struct VertexElementD3DMapping< POS2 >
	{
		static const int D3DDECLTYPE		= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD		= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE		= D3DDECLUSAGE_POSITION;
		static const int USAGE_INDEX		= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(POS2);
	};

	template<>
	struct VertexElementD3DMapping< POS3 >
	{
		static const int D3DDECLTYPE		= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD		= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE		= D3DDECLUSAGE_POSITION;
		static const int USAGE_INDEX		= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(POS3);
	};

	template<>
	struct VertexElementD3DMapping< POS4 >
	{
		static const int D3DDECLTYPE		= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD		= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE		= D3DDECLUSAGE_POSITION;
		static const int USAGE_INDEX		= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(POS4);
	};

	template<>
	struct VertexElementD3DMapping< POST >
	{
		static const int D3DDECLTYPE		= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD		= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE		= D3DDECLUSAGE_POSITIONT;
		static const int USAGE_INDEX		= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(POST);
	};

	template<>
	struct VertexElementD3DMapping< NORMAL >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_NORMAL;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(NORMAL);
	};

	template<>
	struct VertexElementD3DMapping< BINORMAL >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_BINORMAL;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(BINORMAL);
	};

	template<>
	struct VertexElementD3DMapping< TANGENT >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TANGENT;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(TANGENT);
	};

	template<>
	struct VertexElementD3DMapping< DIFFUSE >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_COLOR;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(DIFFUSE);
	};

	template<>
	struct VertexElementD3DMapping< BLENDINDICES<0> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_SHORT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_BLENDINDICES;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(BLENDINDICES<0>);
	};

	template<>
	struct VertexElementD3DMapping< BLENDWEIGHT<0> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_BLENDWEIGHT;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(BLENDWEIGHT<0>);
	};

//
	template<>
	struct VertexElementD3DMapping< UV1<0> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<0>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<0> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<0>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<0> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<0>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<0> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 0;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<0>);
	};


//
	template<>
	struct VertexElementD3DMapping< UV1<1> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 1;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<1>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<1> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 1;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<1>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<1> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 1;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<1>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<1> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 1;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<1>);
	};

//
	template<>
	struct VertexElementD3DMapping< UV1<2> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 2;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<2>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<2> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 2;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<2>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<2> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 2;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<2>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<2> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 2;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<2>);
	};


//
	template<>
	struct VertexElementD3DMapping< UV1<3> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 3;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<3>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<3> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 3;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<3>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<3> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 3;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<3>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<3> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 3;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<3>);
	};

//
	template<>
	struct VertexElementD3DMapping< UV1<4> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 4;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<4>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<4> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 4;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<4>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<4> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 4;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<4>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<4> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 4;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<4>);
	};


//
	template<>
	struct VertexElementD3DMapping< UV1<5> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 5;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<5>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<5> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 5;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<5>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<5> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 5;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<5>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<5> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 5;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<5>);
	};

//
	template<>
	struct VertexElementD3DMapping< UV1<6> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 6;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<6>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<6> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 6;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<6>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<6> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 6;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<6>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<6> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 6;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<6>);
	};


//
	template<>
	struct VertexElementD3DMapping< UV1<7> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT1;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 7;
		static const int ADDITIONAL_OFFSET	= sizeof(UV1<7>);
	};

	template<>
	struct VertexElementD3DMapping< UV2<7> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT2;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 7;
		static const int ADDITIONAL_OFFSET	= sizeof(UV2<7>);
	};

	template<>
	struct VertexElementD3DMapping< UV3<7> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT3;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 7;
		static const int ADDITIONAL_OFFSET	= sizeof(UV3<7>);
	};

	template<>
	struct VertexElementD3DMapping< UV4<7> >
	{
		static const int D3DDECLTYPE	= D3DDECLTYPE_FLOAT4;
		static const int D3DDECLMETHOD	= D3DDECLMETHOD_DEFAULT;
		static const int D3DDECLUSAGE	= D3DDECLUSAGE_TEXCOORD;
		static const int USAGE_INDEX	= 7;
		static const int ADDITIONAL_OFFSET	= sizeof(UV4<7>);
	};




}