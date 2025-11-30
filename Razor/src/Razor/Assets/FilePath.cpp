#include "FilePath.h"
#pragma message("Compiling FilePath.cpp")

namespace Razor
{
	FilePath::FilePath()
		: _mPath(std::string())
	{
	}

	FilePath::FilePath(const std::string& path)
		: _mPath(path)
	{
		// Sanitize path to use forward slashes
		while(_mPath.find('\\') != std::string::npos)
		{
			_mPath.replace(_mPath.find('\\'), 1, "/");
		}
	}

	FilePath::~FilePath()
	{
	}

	FilePath FilePath::RemoveFromPath(FilePath path)
	{
		std::string newPath = _mPath;
		size_t pos = newPath.find(path);
		if (pos != std::string::npos)
		{
			newPath.erase(pos, static_cast<std::string>(path).length());
		}
		return FilePath(newPath);
	}

}