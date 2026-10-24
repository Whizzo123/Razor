#pragma once

#include <filesystem>
#include "Razor.h"

namespace EdgeEditor
{
    bool CopyFile(const Razor::FilePath& source, const Razor::FilePath& dest) {
        bool ret = false;

        try {
            ret = std::filesystem::copy_file(source, dest);
        } catch (std::filesystem::filesystem_error& e) {
            RZ_ERROR("ProjectCreator::CopyFile -> Failed to copy file from: {0}, to: {1}, error: {2}", static_cast<std::string>(source), static_cast<std::string>(dest), e.what());
            ret = false;
        }
        return ret;
    }

    bool CreateDirectory(const Razor::FilePath& dir) {
        bool ret = false;

        try {
            ret = std::filesystem::create_directory(dir);
        } catch (std::filesystem::filesystem_error& e) {
            RZ_ERROR("ProjectCreator::CreateDirectory -> Failed to create directory: {0}, error: {1}", static_cast<std::string>(dir), e.what());
            ret = false;
        }
        return ret;
    }
}