#include "ProjectCreator.h"
#include <filesystem>
#include <iterator>
#include <string>
#include <fstream>


namespace EdgeEditor {

bool ProjectCreator::Create(const Razor::FilePath& projectDir) {
    static Razor::FilePath templateDir("template", true);
    std::string projectName = "Test";

    bool ret = true;

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

    Razor::FilePath projectFilePath = projectDir + Razor::FilePath(projectName + ".proj");
    Razor::FilePath cmakeListsFilePath = projectDir + Razor::FilePath("CMakeLists.txt");
    Razor::FilePath premakeFilePath = projectDir + Razor::FilePath("scripts/premake5.lua");
    Razor::FilePath generateBatFilePath = projectDir + Razor::FilePath("scripts/GenerateProjects.bat");
    Razor::FilePath generateShFilePath = projectDir + Razor::FilePath("scripts/GenerateProjects.sh");

    // .proj
    ret |= CopyFile(templateDir + Razor::FilePath("project/Template.proj"), projectFilePath);
    // CMakeLists.txt / premake5.lua
    ret |= CopyFile(templateDir + Razor::FilePath("project/CMakeLists.txt"), cmakeListsFilePath);
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/premake5.lua"), premakeFilePath);
    // GenerateProjects.bat?? GenerateProjects.sh?? These look to be for premake only
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/GenerateProjects.bat"), generateBatFilePath);
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/GenerateProjects.sh"), generateShFilePath);

    //Replacement part
    ret |= Replace(projectFilePath, "@ProjectName", projectName);
    ret |= Replace(cmakeListsFilePath, "@ProjectName", projectName);
    ret |= Replace(premakeFilePath, "@ProjectName", projectName);

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

bool ProjectCreator::Replace(Razor::FilePath& file, const std::string& patternText, const std::string& replacementText) {
    std::ifstream in(file);

    if (!in) {
        RZ_ERROR("ProjectCreator::Replace -> Failed to open file for reading: {0}", static_cast<std::string>(file));
        return false;
    }

    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    size_t len = patternText.length();
    size_t pos = 0;

    while ((pos = text.find(patternText, pos)) != std::string::npos) {
        text.replace(pos, len, replacementText);
        pos += replacementText.length();
    }

    std::ofstream out(file, std::ios::trunc);
    if (!out) {
        RZ_ERROR("ProjectCreator::Replace -> Failed to open file for writing: {0}", static_cast<std::string>(file));
        return false;
    }

    out.write(text.data(), static_cast<std::streamsize>(text.size()));
    return static_cast<bool>(out);
}

}