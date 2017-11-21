#include "resourceShaderGLES.h"
#include "textFile/library.include.h"
#include "resourceUtils/library.include.h"

#include <fstream>
#include <unordered_map>

namespace Resources
{
	void ShaderGLSL::Clear()
	{
		deactivate();
		compiler = opengl::ShaderProgramCompiler();
	}

	void ShaderGLSL::Compile(stream::ostream& os)
	{
		auto s = serializer(os);
		compiler.serialize(s);
	}

	std::unordered_map<std::string, GLenum> dict;
	std::once_flag flag;

	GLenum extdict(const std::string& ext)
	{
		std::call_once(flag, [] 
		{
			dict[".vs"] = GL_VERTEX_SHADER;
			dict[".fs"] = GL_FRAGMENT_SHADER;
		});

		return dict[ext];
	}

	void ShaderGLSL::LoadSource(const char* sourceFile, const UserData* userData)
	{
		Resources::call_line_by_line(sourceFile, [sourceFile, this](const boost::filesystem::path& line)
		{
			auto path = boost::filesystem::path(sourceFile).remove_filename() / line;
			dependencies.addActualDependence(path, line);
			TextFile::TextFromFile text(path.string().c_str());

			int len;
			const char* txt;
			text.text(&txt, &len);
			compiler.attach(extdict(line.extension().string()), txt, path.string(), 0);
		});
	}

	void ShaderGLSL::LoadCompiled(stream::istream& is)
	{
		auto s = serializer(is);
		compiler.serialize(s);
		compiler.compile(nativeResource);

		activate();
	}

	bool ShaderGLSL::set()
	{
		return resource([](auto& res) { res.set(); });
	}
}
