#include "ProjectBuilder.h"
#include <cstdlib>
#include <filesystem>
#include "../Utils/IO/FileUtils.h"
#include <format>

namespace EdgeEditor 
{
    bool ProjectBuilder::BuildProject(const Razor::FilePath &projectDir) {
        //SDK - Fix this path once we have fixed up how we deal with file paths
        if (CopyFolder(Razor::FilePath("bin/Debug-linux-x86_64/Edge/SDK"), projectDir + Razor::FilePath("SDK"), std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing)) {
            RZ_WARN("ProjectBuilder::BuildProject -> Failed to copy SDK to game directory may fail to build/or be out of date");
        }

        std::string config_command = std::format("cmake -S {} -B {} -DCMAKE_BUILD_TYPE=\"Debug\" -DCMAKE_C_COMPILER=\"clang\" -DCMAKE_CXX_COMPILER=\"clang++\"",
            static_cast<std::string>(projectDir), static_cast<std::string>(projectDir) + "/build/cmake");
        const int config_result = std::system(
            config_command.c_str()
        );
        if (config_result != 0) {
            RZ_ERROR("Failed to configure project CMake");
            return false;
        }
        // Execute the build
        std::string build_command = std::format("cmake --build {} --target Sandbox", static_cast<std::string>(projectDir) + ("/build/cmake"));
        const int build_result = std::system(
            build_command.c_str()
        );
        if (build_result != 0) {
            RZ_ERROR("Failed to build project CMake");
            return false;
        }
        return true;
    }
}