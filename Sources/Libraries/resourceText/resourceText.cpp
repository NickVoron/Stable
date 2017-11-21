#include "resourceText.h"
#include "textFile/library.include.h"	


namespace Resources
{
	void Text::Clear()
	{
		deactivate();
		nativeResource.clear();
	}

	void Text::Compile(stream::ostream& os)
	{
		os << nativeResource;
	}

	void Text::LoadSource(const char* sourceFile, const UserData* userData)
	{
		nativeResource = TextFile::TextFromFile(sourceFile).text();
	}

	void Text::LoadCompiled(stream::istream& is)
	{
		is >> nativeResource;
		activate();
	}
}


