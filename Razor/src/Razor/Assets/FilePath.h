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

		operator std::string() const
		{
			return _mPath;
		}
	private:
		std::string _mPath;
	};
}

