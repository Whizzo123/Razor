#pragma once

#include <string>
#include "../Core.h"

namespace Razor
{
	class RAZOR_API FilePath
	{

	public:
		FilePath();
		FilePath(const std::string& path);
		~FilePath();

		FilePath RemoveFromPath(FilePath path);

		FilePath operator+(const FilePath& other) const
		{
			return FilePath(_mPath + "/" + other._mPath);
		}

		operator std::string() const
		{
			return _mPath;
		}
	private:
		std::string _mPath;
	};
}

