//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <string>
#include <vector>

namespace arxml::project {

    class ModelProject {
    public:
        ModelProject() = default;

        bool addFile(const std::string& file);
        bool addDirectory(const std::string& directory);
        [[nodiscard]] const std::vector<std::string>& getFileList() const { return m_files; }
    private:
        std::vector<std::string> m_files;
    };
    
}
