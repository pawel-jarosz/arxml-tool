//
// Created by Paweł Jarosz on 11.01.2024.
//

#include <arxml_tool/project_content_reader.hpp>

#include <yaml-cpp/yaml.h>

namespace arxml_tool {

    void YamlProjectFileParser::parse() {
        YAML::Node config = YAML::Load(m_input.getContent());
        const auto& file_list = config["FileList"];
        if (file_list) {
            for (int it = 0; it < file_list.size(); ++it) {
                m_files.push_back(file_list[it].as<std::string>());
            }
        }
        const auto& directory_list = config["DirList"];
        if (file_list) {
            for (int it = 0; it < directory_list.size(); ++it) {
                m_files.push_back(directory_list[it].as<std::string>());
            }
        }
    }

}
