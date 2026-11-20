#include "ProjectCreator.h"
#include <filesystem>
#include <iterator>
#include <string>
#include <fstream>
#include "../Utils/IO/FileUtils.h"


namespace EdgeEditor {

bool ProjectCreator::Create(const Razor::FilePath& projectDir) {
    static Razor::FilePath templateDir("template", true);
    std::string projectName = projectDir.GetName();

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
    // src folder
    if (!CreateDirectory(projectDir + Razor::FilePath("src", true))) {
        return false;
    }
    // TODO all paths should resolve to executable directory i.e all folders we access template, SDK, config should all live within the executable directory we can still have them in source just
    // make it part of the build pipeline that they get copied into the bin/<type>/Edge folder future thing but soon future thing can just absolute path SDK for now
    Razor::FilePath projectFilePath = projectDir + Razor::FilePath(projectName + ".proj");
    Razor::FilePath cmakeListsFilePath = projectDir + Razor::FilePath("CMakeLists.txt");
    Razor::FilePath premakeFilePath = projectDir + Razor::FilePath("scripts/premake5.lua");
    Razor::FilePath generateBatFilePath = projectDir + Razor::FilePath("scripts/GenerateProjects.bat");
    Razor::FilePath generateShFilePath = projectDir + Razor::FilePath("scripts/GenerateProjects.sh");
    Razor::FilePath srcFilePath = projectDir + Razor::FilePath("src/" + projectName + ".cpp");

    // .proj
    ret |= CopyFile(templateDir + Razor::FilePath("project/Template.proj"), projectFilePath);
    // CMakeLists.txt / premake5.lua
    ret |= CopyFile(templateDir + Razor::FilePath("project/CMakeLists.txt"), cmakeListsFilePath);
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/premake5.lua"), premakeFilePath);
    // GenerateProjects.bat?? GenerateProjects.sh?? These look to be for premake only
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/GenerateProjects.bat"), generateBatFilePath);
    ret |= CopyFile(templateDir + Razor::FilePath("project/scripts/GenerateProjects.sh"), generateShFilePath);

    ret |= CopyFile(templateDir + Razor::FilePath("project/src/TemplateApp.cpp"), srcFilePath);

    //Replacement part
    ret |= Replace(projectFilePath, "@ProjectName", projectName);
    ret |= Replace(cmakeListsFilePath, "@ProjectName", projectName);
    ret |= Replace(premakeFilePath, "@ProjectName", projectName);
    ret |= Replace(srcFilePath, "@ProjectName", projectName);
    ret |= Replace(srcFilePath, "@FilePath", projectFilePath);

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