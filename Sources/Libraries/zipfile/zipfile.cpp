#include "zipfile.h"

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

