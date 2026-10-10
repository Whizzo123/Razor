#pragma once

#include <string>

#include "Razor.h"

namespace EdgeEditor {

class ProjectCreator {

public:
    // Single entry point create function
    static bool Create(const Razor::FilePath& projectDir);
private:
    static bool CopyFile(Razor::FilePath& source, Razor::FilePath& dest);
    static bool Replace(std::string& text, const std::string& patternText, const std::string& replacementText);
};

}
