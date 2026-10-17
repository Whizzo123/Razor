#include "ProjectBuilder.h"
#include <cstdlib>
#include <format>

namespace EdgeEditor 
{
    bool ProjectBuilder::BuildProject(const Razor::FilePath &projectDir) {
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