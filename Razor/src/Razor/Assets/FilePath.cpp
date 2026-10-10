#include "FilePath.h"
#pragma message("Compiling FilePath.cpp")

namespace Razor
{
	FilePath::FilePath()
		: _mPath(std::string()), mbIsDirectory(false)
	{
	}

	FilePath::FilePath(const std::string& path, bool bIsDirectory)
		: _mPath(path), mbIsDirectory(bIsDirectory)
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

	FilePath FilePath::RemoveFromPath(FilePath path) const
	{
		std::string newPath = _mPath;
		size_t pos = newPath.find(path);
		if (pos != std::string::npos)
		{
			newPath.erase(pos, static_cast<std::string>(path).length() + 1);
		}
		return FilePath(newPath);
	}

	bool FilePath::IsDir() const
	{
		return mbIsDirectory;
	}

	std::optional<FilePath> FilePath::GetParent() const {
		size_t pos = _mPath.find_last_of('/');
		if (pos != std::string::npos) {
			return FilePath(_mPath.substr(0, pos), true);
		}
		return std::nullopt;
	}
}