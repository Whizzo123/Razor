#include "ProjectCreator.h"
#include <filesystem>


namespace EdgeEditor {

bool ProjectCreator::Create(const Razor::FilePath& projectDir) {
    static Razor::FilePath templateDir("template", true);
    std::string projectName = "Test";

    bool ret = false;

    // project folder
    if (!CreateDirectory(projectDir)) {
        return false;
    }
    // assets folder
    if (!CreateDirectory(projectDir + Razor::FilePath("assets", true))) {
        return false;
    }
    // scripts folder
    if (!CreateDirectory(projectDir + Razor::FilePath("scripts", true))) {
        return false;
    }
    // .proj
    ret = CopyFile(templateDir + Razor::FilePath("project/Template.proj"), projectDir + Razor::FilePath(projectName + ".proj"));
    // CMakeLists.txt / premake5.lua
    ret |= CopyFile(templateDir + Razor::FilePath("project/CMakeLists.txt"), projectDir + Razor::FilePath("CMakeLists.txt"));
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/premake5.lua"), projectDir + Razor::FilePath("scripts/premake5.lua"));
    // GenerateProjects.bat?? GenerateProjects.sh?? These look to be for premake only
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/GenerateProjects.bat"), projectDir + Razor::FilePath("scripts/GenerateProjects.bat"));
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/GenerateProjects.sh"), projectDir + Razor::FilePath("scripts/GenerateProjects.sh"));

    //Replacement part

    return ret;
}

bool ProjectCreator::CopyFile(const Razor::FilePath& source, const Razor::FilePath& dest) {
    bool ret = false;

    try {
        ret = std::filesystem::copy_file(source, dest);
    } catch (std::filesystem::filesystem_error& e) {
        RZ_ERROR("ProjectCreator::CopyFile -> Failed to copy file from: {0}, to: {1}, error: {2}", static_cast<std::string>(source), static_cast<std::string>(dest), e.what());
        ret = false;
    }
    return ret;
}

bool ProjectCreator::CreateDirectory(const Razor::FilePath& dir) {
    bool ret = false;

    try {
        ret = std::filesystem::create_directory(dir);
    } catch (std::filesystem::filesystem_error& e) {
        RZ_ERROR("ProjectCreator::CreateDirectory -> Failed to create directory: {0}, error: {1}", static_cast<std::string>(dir), e.what());
        ret = false;
    }
    return ret;
}

bool ProjectCreator::Replace(std::string& text, const std::string& patternText, const std::string& replacementText) {
    return true;
}

}