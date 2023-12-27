//
// Created by Paweł Jarosz on 27.12.2023.
//

#include <arxml/project.hpp>

#include <filesystem>

namespace arxml::project {
    bool ModelProject::addDirectory(const std::string& directory) {
        if (not std::filesystem::exists(std::filesystem::path{directory})) {
            return false;
        }
        for (const auto it: std::filesystem::recursive_directory_iterator(directory)) {
            if (it.path().string().ends_with(".arxml")) {
                m_files.emplace_back(std::move(it.path().string()));
            }
        }
        return true;
    }

    bool ModelProject::addFile(const std::string& file) {
        if (not std::filesystem::exists(std::filesystem::path{file})) {
            return false;
        }
        m_files.push_back(file);
        return true;
    }
}