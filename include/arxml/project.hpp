//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <string>
#include <vector>

#include <arxml/utilities/parser_facade.hpp>

namespace arxml::project {

    class ModelProject {
    public:
        ModelProject() = default;

        bool addFile(const std::string& file);
        bool addDirectory(const std::string& directory);
        [[nodiscard]] const std::vector<std::string>& getFileList() const { return m_files; }

        template<typename ParserFacade>
        static void openModelFromProject(const ModelProject& project, ParserFacade& facade);
    private:
        std::vector<std::string> m_files;
    };

    template<typename ParserFacade>
    void ModelProject::openModelFromProject(const arxml::project::ModelProject& project, ParserFacade& facade) {
        for (const auto& it: project.getFileList()) {
            facade.parse(it);
        }
    }

}
