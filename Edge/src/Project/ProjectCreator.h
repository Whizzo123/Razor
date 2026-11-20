#pragma once

#include <string>

#include "Razor.h"

namespace EdgeEditor {

class ProjectCreator {

public:
    // Single entry point create function
    static bool Create(const Razor::FilePath& projectDir);
private:
    static bool Replace(Razor::FilePath& file, const std::string& patternText, const std::string& replacementText);
};

}
