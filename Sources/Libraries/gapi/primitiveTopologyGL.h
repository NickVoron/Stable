// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#ifdef USE_WINDOWS
#include "primitiveTopology.h"
#include "shaders.h"

#include "glew/library.include.h"

namespace gapi
{
 	template<> struct Topology<GL, POINTLIST>		{ static const GLenum topology = GL_POINTS;			};
 	template<> struct Topology<GL, LINELIST>		{ static const GLenum topology = GL_LINES;			};
 	template<> struct Topology<GL, LINESTRIP>		{ static const GLenum topology = GL_LINE_STRIP;		};
 	template<> struct Topology<GL, TRIANGLELIST>	{ static const GLenum topology = GL_TRIANGLES;		};
 	template<> struct Topology<GL, TRIANGLESTRIP>	{ static const GLenum topology = GL_TRIANGLE_STRIP;	};

 	template<GLenum topology> struct TopologyGL;
 	template<GLenum topology> struct PrimitiveCountEvaluatorGL { static inline int eval(int indicesCount) { return PrimitiveCountEvaluator< TopologyGL<topology>::topology >::eval(indicesCount); } };
 
  	template<>	struct TopologyGL<GL_POINTS>			{ static const PrimitiveTopology topology = POINTLIST;		};
  	template<>	struct TopologyGL<GL_LINES>				{ static const PrimitiveTopology topology = LINELIST;		};
  	template<>	struct TopologyGL<GL_LINE_STRIP>		{ static const PrimitiveTopology topology = LINESTRIP;		};
  	template<>	struct TopologyGL<GL_TRIANGLES>			{ static const PrimitiveTopology topology = TRIANGLELIST;	};
  	template<>	struct TopologyGL<GL_TRIANGLE_STRIP>	{ static const PrimitiveTopology topology = TRIANGLESTRIP;	};

 	template<ShaderTypeName shaderType> struct ShaderType;
 	template<> struct ShaderType<VERTEX_SHADER>		{ static const GLenum type = GL_VERTEX_SHADER; };
 	template<> struct ShaderType<PIXEL_SHADER>		{ static const GLenum type = GL_FRAGMENT_SHADER; };
#if ENABLE_OPENGL
	template<> struct ShaderType<GEOMETRY_SHADER> { static const GLenum type = GL_GEOMETRY_SHADER; };
	template<> struct ShaderType<HULL_SHADER> { static const GLenum type = GL_TESS_CONTROL_SHADER; };
	template<> struct ShaderType<DOMAIN_SHADER> { static const GLenum type = GL_TESS_EVALUATION_SHADER; };
#endif
 	
}

#endif




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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