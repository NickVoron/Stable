#pragma once

#include <string>
#include "stream/library.include.h"

namespace texutils {
namespace DXT {

	void Compress(const std::string& inputFileName, const std::string& outputFileName, bool isNormalMap, bool dxt5, bool genMips);
	void Compress(const std::string& inputFileName, stream::ostream& os, bool isNormalMap, bool dxt5, bool genMips);
	
} //
} //

