// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <boost/filesystem.hpp>

#include "stream/library.include.h"

#include "stuff/fileModifyObserver.h"


namespace Resources 
{
	struct Dependencies
	{
		struct FileInfo 
		{
			Base::FileUtils::file_time time;
			std::string absoluteFileName;
			std::string relativeFileName;

			bool operator < (const FileInfo& fi) const { return relativeFileName < fi.relativeFileName; }
		};

		struct ActualFileInfo
		{
			auto time() const { return observer.time;	}
			std::string absoluteFileName() const;
			std::string relativeFileName() const;

			void set(const std::string& absFile, const std::string& relFile);
			bool refresh();

			Base::FileUtils::ModifyObserver observer;
			std::string relativeFile;
		};


		void setActualRoot(const boost::filesystem::path& d);
		void addActualDependence(const boost::filesystem::path& absFile, const boost::filesystem::path& relFile);
		bool checkActualModified();
		bool equalWithStored();
		bool valid() const;

		void refreshStored();
		void refreshActual();

		typedef std::map<std::string, FileInfo> FileInfoList;
		typedef std::map<std::string, ActualFileInfo> Observers;

		struct ActualState;
		struct StoredState;

		struct ActualState 
		{
			ActualFileInfo root;
			Observers dependencies;

			void refresh(const StoredState& as);
			bool refresh();
		};

		struct StoredState 
		{
			FileInfo root;
			FileInfoList dependencies;

			void refresh(const ActualState& as);
		};

		static bool equal(const FileInfo& fi, const ActualFileInfo& afi);
		static bool equal(const StoredState& ss, const ActualState& as);
		
		ActualState actual;
		StoredState stored;
	};

	stream::ostream& operator <<(stream::ostream& s, const Dependencies::StoredState& ss);
	stream::istream& operator >>(stream::istream& s, Dependencies::StoredState& ss);

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