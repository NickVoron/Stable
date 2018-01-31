// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "zipfile.h"

#include <stdexcept>

namespace zip
{

	File::File() : z(0)
	{

	}

	File::File(const std::string& filename) : z(0)
	{
		open(filename);
	}

	File::~File()
	{
		close();
	}

	void File::open(const std::string& filename)
	{
		close();

		z = unzOpen(filename.c_str());
		if (!z) 
		{
			throw std::runtime_error("cannot open file " + filename);
		}
	}

	void File::close()
	{
		if (z) 
		{
			unzClose(z);
			z = 0;
		}
	}

	bool File::opened() const
	{
		return z != 0;
	}

	void File::files(std::vector<std::string>& vec)
	{

	}

	bool File::exists(const std::string& filename)
	{
		if (!opened())
		{
			throw std::runtime_error("archive is not opened");
			return false;
		}

		return unzLocateFile(z, filename.c_str(), 0) == UNZ_OK;
	}

	void File::readFile(const std::string& filename, std::vector<char>& data)
	{
		if (!opened())
		{
			throw std::runtime_error("archive is not opened");
		}

		if (unzLocateFile(z, filename.c_str(), 0) != UNZ_OK) 
		{
			throw std::runtime_error("cannot locate file " + filename);
		}

		unz_file_info info;
		if (unzGetCurrentFileInfo(z, &info, 0, 0, 0, 0, 0, 0) != UNZ_OK) 
		{
			throw std::runtime_error("cannot get info for file " + filename);
		}

		if (unzOpenCurrentFile(z) != UNZ_OK) 
		{
			throw std::runtime_error("cannot open file " + filename);
		}

		data.resize(info.uncompressed_size);

		if (unzReadCurrentFile(z, &data[0], info.uncompressed_size) != int(info.uncompressed_size)) 
		{
			unzCloseCurrentFile(z);
			throw std::runtime_error("cannot read file " + filename);
		}

		if (unzCloseCurrentFile(z) != UNZ_OK) 
		{
			throw std::runtime_error("cannot close file " + filename);
		}

	}
}





// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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