// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
		compiler = ShaderProgramCompiler();
	}

	void ShaderGLSL::Compile(stream::ostream& os)
	{
		compiler.serialize(serializer(os));
	}

	GLenum extdict(const std::string& ext)
	{
		static std::unordered_map<std::string, GLenum> dict;
		static std::once_flag flag;
		std::call_once(flag, [] 
		{
			dict[".vs"] = GL_VERTEX_SHADER;
			dict[".fs"] = GL_FRAGMENT_SHADER;
		});

		return dict[ext];
	}

	void ShaderGLSL::LoadSource(const char* sourceFile, const void* userData, std::size_t userDataSize)
	{
		Resources::call_line_by_line(sourceFile, [sourceFile, this](const boost::filesystem::path& line)
		{
			dependencies.addActualDependence(line, line);

			auto path = boost::filesystem::path(sourceFile).remove_filename() / line;

			TextFile::TextFromFile text(path.string().c_str());

			int len;
			const char* txt;
			text.text(&txt, &len);
			compiler.attach(extdict(line.extension().string()), txt);
		});
	}

	void ShaderGLSL::LoadCompiled(stream::istream& is)
	{
		compiler.serialize(serializer(is));
		compiler.compile(nativeResource);

		activate();
	}

	bool ShaderGLSL::set()
	{
		return resource([](auto& res) { res.set(); });
	}
}




// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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