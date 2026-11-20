#pragma once

#include <filesystem>
#include "Razor.h"

namespace EdgeEditor
{
    static bool CopyFile(const Razor::FilePath& source, const Razor::FilePath& dest) {
        bool ret = false;

        try {
            ret = std::filesystem::copy_file(source, dest);
        } catch (std::filesystem::filesystem_error& e) {
            RZ_ERROR("CopyFile -> Failed to copy file from: {0}, to: {1}, error: {2}", static_cast<std::string>(source), static_cast<std::string>(dest), e.what());
            ret = false;
        }
        return ret;
    }

    static bool CopyFolder(const Razor::FilePath& source, const Razor::FilePath& dest, std::filesystem::copy_options options) {
        bool ret = true;
        std::error_code err;
        try {
            std::filesystem::copy(source, dest, options, err);
        } catch (std::filesystem::filesystem_error& e) {
            RZ_ERROR("CopyFolder -> Failed to copy folder from: {0}, to: {1}, error: {2}", static_cast<std::string>(source), static_cast<std::string>(dest), e.what());
            ret = false;
        }
        if (err) {
            RZ_ERROR("CopyFolder -> Copying folder from: {0}, to {1}, resulted in error: {2}", static_cast<std::string>(source), static_cast<std::string>(dest), err.message());
            ret = false;
        }

        return ret;
    }

    static bool CreateDirectory(const Razor::FilePath& dir) {
        bool ret = false;

        try {
            ret = std::filesystem::create_directory(dir);
        } catch (std::filesystem::filesystem_error& e) {
            RZ_ERROR("CreateDirectory -> Failed to create directory: {0}, error: {1}", static_cast<std::string>(dir), e.what());
            ret = false;
        }
        return ret;
    }
}