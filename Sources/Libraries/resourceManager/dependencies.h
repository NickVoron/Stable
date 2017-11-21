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
			time_t time;
			std::string absoluteFileName;
			std::string relativeFileName;

			bool operator < (const FileInfo& fi) const { return relativeFileName < fi.relativeFileName; }
		};

		struct ActualFileInfo
		{
			time_t time() const;
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
