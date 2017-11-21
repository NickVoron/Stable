#pragma once

#include "stuff/nonCopyable.h"

#include "minizip/library.include.h"

#include <string>
#include <vector>

namespace zip
{
	class File : public Base::NonCopyable
	{
	public:
		File();
		File(const std::string& filename);

		~File();

		void open(const std::string& filename);
		void close();
		
		bool opened() const;
		
		void files(std::vector<std::string>& vec);
		bool exists(const std::string& filename);
		void readFile(const std::string& filename, std::vector<char>& data);

	private:
		unzFile z;
	};
}

