#pragma once

#include <string>
#include <sys/stat.h>

namespace Base 
{
namespace FileUtils 
{

class ModifyObserver 
{
public:
	static bool enabled;

	// maintenance:
	ModifyObserver() {}
	ModifyObserver( const std::string& fn ){ SetFileName( fn ); }
	ModifyObserver( const ModifyObserver& mo ):fileName(mo.fileName), time(mo.time) {}

	// usage:
	bool CheckModified() const;
	bool CheckAndClearModified();
	void ClearModified();
	void SetFileName( const std::string& fn );
	
	// for std::map
	bool operator < ( const ModifyObserver& a ) const {return fileName < a.fileName; }

	std::string fileName;
	time_t time;
};

} // -namespace
}