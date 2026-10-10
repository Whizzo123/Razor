#include "ProjectCreator.h"
#include <filesystem>


namespace EdgeEditor {

bool ProjectCreator::Create(const Razor::FilePath& projectDir) {
    if (!std::filesystem::create_directory(static_cast<std::string>(projectDir))) {
        RZ_ERROR("ProjectCreator::Create -> Failed to create project folder at given path: {0}", static_cast<std::string>(projectDir));
        return false;
    }
    // project folder
    // .proj
    // CMakeLists.txt / premake5.lua
    // assets folder
    // Scripts folder
    // GenerateProjects.bat?? GenerateProjects.sh?? These look to be for premake only
    return true;
}

bool ProjectCreator::CopyFile(Razor::FilePath& source, Razor::FilePath& dest) {
    return true;
}

bool ProjectCreator::Replace(std::string& text, const std::string& patternText, const std::string& replacementText) {
    return true;
}

}