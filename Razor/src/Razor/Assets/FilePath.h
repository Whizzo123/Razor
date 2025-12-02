#pragma once

#include <string>
#include "../Core.h"

namespace Razor
{
	class RAZOR_API FilePath
	{

	public:
		FilePath();
		FilePath(const std::string& path, bool bIsDirectory = false);
		~FilePath();

		FilePath RemoveFromPath(FilePath path) const;

		FilePath operator+(const FilePath& other) const
		{
			bool bIsDirectory = IsDir() && other.IsDir();
			return FilePath(_mPath + "/" + other._mPath, bIsDirectory);
		}

		operator std::string() const
		{
			return _mPath;
		}

		bool IsDir() const;
	private:
		std::string _mPath;
		bool mbIsDirectory;
	};
}

