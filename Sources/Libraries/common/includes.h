#pragma once
#include <boost/filesystem.hpp>
#include <set>


struct Includes : public std::set<boost::filesystem::path>
{
	template<class Function>
	bool add(const boost::filesystem::path& path, Function&& function);
	bool exist(const boost::filesystem::path& path);
};

inline bool Includes::exist(const boost::filesystem::path& path)
{
	for (auto& it : *this)
	{
		if (boost::filesystem::equivalent(path, it))
		{
			return true;
		}
	}

	return false;
}

template<class Function>
inline bool Includes::add(const boost::filesystem::path& path, Function&& function)
{
	if (!exist(path))
	{
		function(path);
		insert(path);
		return true;
	}

	return false;
}