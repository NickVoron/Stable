// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "dependencies.h"

namespace Resources 
{
	
	
	
	std::string Dependencies::ActualFileInfo::absoluteFileName() const
	{
		return observer.fileName;
	}

	std::string Dependencies::ActualFileInfo::relativeFileName() const
	{
		return relativeFile;
	}

	void Dependencies::ActualFileInfo::set(const std::string& absFile, const std::string& relFile)
	{
		observer.SetFileName(absFile);
		relativeFile = relFile;
	}

	bool Dependencies::ActualFileInfo::refresh()
	{
		return observer.CheckAndClearModified();
	}

	void Dependencies::refreshStored(){stored.refresh(actual);}
	void Dependencies::refreshActual(){actual.refresh(stored);}

	
	void Dependencies::StoredState::refresh(const ActualState& as)
	{
		root.time = as.root.time();
		root.absoluteFileName = as.root.absoluteFileName();
		root.relativeFileName = as.root.relativeFileName();

		for(const Observers::value_type& o : as.dependencies)
		{
			FileInfo& fileInfo = dependencies[o.first];

			fileInfo.time		= o.second.time();
			fileInfo.absoluteFileName = o.second.absoluteFileName();
			fileInfo.relativeFileName = o.second.relativeFileName();
		}
	}

	void Dependencies::ActualState::refresh(const StoredState& ss)
	{
		root.set(ss.root.absoluteFileName, ss.root.relativeFileName);

		for(const FileInfoList::value_type& f : ss.dependencies)
		{
			dependencies[f.first].set(f.second.absoluteFileName, f.second.relativeFileName);
		}
	}

	bool Dependencies::ActualState::refresh()
	{
		if (root.refresh())
			return true;

		for (Observers::value_type& o : dependencies)
		{
			if (o.second.refresh())
				return true;
		}

		return false;
	}

	
	bool Dependencies::equal(const FileInfo& fi, const ActualFileInfo& afi)
	{
		return (fi.time != afi.time()) || (fi.absoluteFileName != afi.absoluteFileName()) || (fi.relativeFileName != afi.relativeFileName());
	}

	bool Dependencies::equal(const StoredState& ss, const ActualState& as)
	{
		if(equal(ss.root, as.root)) return false;
		if(as.dependencies.size() != ss.dependencies.size()) return false;

		for(const Observers::value_type& o : as.dependencies)
		{
			FileInfoList::const_iterator i = ss.dependencies.find(o.first);
			if(i != ss.dependencies.end())
			{
				if(equal(i->second, o.second) )
					return false;
			}			
		}

		return true;
	}

	
	
	
	void Dependencies::setActualRoot(const boost::filesystem::path& d)
	{
		actual.root.set(d.string(), "");
	}

	void Dependencies::addActualDependence(const boost::filesystem::path& absFile, const boost::filesystem::path& relFile)
	{
		actual.dependencies[absFile.string()].set(absFile.string(), relFile.string());
	}

	bool Dependencies::checkActualModified()
	{
		return actual.refresh();
	}

	bool Dependencies::valid() const
	{
		return !actual.root.observer.fileName.empty();
	}

	bool Dependencies::equalWithStored()
	{
		return equal(stored, actual) && valid();
	}

} 





// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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