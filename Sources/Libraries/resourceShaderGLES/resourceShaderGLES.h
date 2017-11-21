#pragma once

#include "resourceManager/library.include.h"

#include "opengles/library.include.h"
#include "opengl/library.include.h"

namespace Resources
{
	class ShaderGLSL : public ResourceT<ShaderGLSL, opengl::ShaderProgram>
	{
	public:
		inline static const char* basePath() { return "glsl/"; }
		inline static const char* TypeName() { return "Shader GLSL"; }

		virtual void Clear();

		bool set();
								
	protected:
		virtual void Compile(stream::ostream& os) override;
		virtual void LoadSource(const char* sourceFile, const UserData* userData) override;
		virtual void LoadCompiled(stream::istream& is) override;

	private:
		opengl::ShaderProgramCompiler compiler;
	};
}