#pragma once

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