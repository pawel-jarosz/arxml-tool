//
// Created by Paweł Jarosz on 04.01.2024.
//

#pragma once

#include <arxml/utilities/input_source.hpp>
#include <arxml/project.hpp>

namespace arxml_tool {

    class AbstractProjectFileParser {
    public:
        virtual ~AbstractProjectFileParser() = default;
        const std::vector<std::string>& getImplicitFiles() const { return m_files; }
        const std::vector<std::string>& getDirectories() const { return m_directories; }

        virtual void parse() = 0;
    protected:
        std::vector<std::string> m_files;
        std::vector<std::string> m_directories;
    };

    class YamlProjectFileParser : public AbstractProjectFileParser {
    public:
        YamlProjectFileParser(arxml::utilities::io::IInputSource& input)
        : m_input{input} {

        }

        void parse() override;
    private:
        arxml::utilities::io::IInputSource& m_input;
    };

    template<class ProjectFileParser>
    class ProjectContentReader {
    public:
        explicit ProjectContentReader(arxml::project::ModelProject& project)
        : m_project{project}
        , m_config_parser{}
        {}

        void read(arxml::utilities::io::IInputSource& source);

    protected:
        ProjectFileParser& getConfigurationParser() { return m_config_parser; }

    private:
        arxml::project::ModelProject& m_project;
        std::unique_ptr<AbstractProjectFileParser> m_config_parser;
    };


    template<class ProjectFileParser>
    void ProjectContentReader<ProjectFileParser>::read(arxml::utilities::io::IInputSource& source) {
        m_config_parser = std::make_unique<ProjectFileParser>(source);
        const auto& implicit_files = m_config_parser->getImplicitFiles();
        std::for_each(std::begin(implicit_files), std::end(implicit_files),
                      [&](const std::string& value) { m_project.addFile(value); });
        const auto& directory = m_config_parser->getDirectories();
        std::for_each(std::begin(directory), std::end(directory),
                      [&](const std::string& value) { m_project.addDirectory(value); });
    }
}
