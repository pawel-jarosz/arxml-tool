//
// Created by Paweł Jarosz on 27.12.2023.
//

#include "finder_subprogram.hpp"

#include <arxml/utilities/parser_facade.hpp>
#include <arxml/helpers/finders.hpp>
#include <arxml/dfs/traversal.hpp>
#include <arxml/printer.hpp>
#include <arxml/project.hpp>

#include <iostream>
#include <sstream>
#include <map>

namespace arxml_tool {
    namespace {

        void find_by_tag(const std::string& path, const std::string& tag) {
            arxml::utilities::DefaultParserFacade parser;
            arxml::project::ModelProject project;
            project.addDirectory(path);
            arxml::project::ModelProject::openModelFromProject(project, parser);
            auto model = parser.getModel();
            std::map<std::string, arxml::model::INamedAutosarElement&> result;
            arxml::helpers::ElementByTagFinder callback(result, tag);
            arxml::dfs::traverse_model(*model, callback);
            std::cout << "Found following entries with " << tag << " tag:\n";
            if (result.empty()) { std::cout << "none\n"; }
            else {
                std::for_each(result.begin(), result.end(),
                              [](auto it) { std::cout << it.first << std::endl; }
                );
            }
        }

        void find_by_id(const std::string& path, const std::string& id) {
            arxml::utilities::DefaultParserFacade parser;
            arxml::project::ModelProject project;
            project.addDirectory(path);
            arxml::project::ModelProject::openModelFromProject(project, parser);
            auto model = parser.getModel();
            std::vector<std::reference_wrapper<arxml::model::INamedAutosarElement>> result;
            arxml::helpers::ElementByIdFinder callback(result, id);
            arxml::dfs::traverse_model(*model, callback);
            std::cout << "Found following entry on path " << id << ":\n";
            if (result.empty()) { std::cout << "none\n"; }
            else {
                arxml::printer::TreePrinter::stdout_dump(result[0]);
            }
        }

        void find_by_ref(const std::string& path, const std::string& id) {
            arxml::utilities::DefaultParserFacade parser;
            arxml::project::ModelProject project;
            project.addDirectory(path);
            arxml::project::ModelProject::openModelFromProject(project, parser);
            auto model = parser.getModel();
            std::vector<std::string> result;
            arxml::helpers::ElementByReferenceFinder callback(result, id);
            arxml::dfs::traverse_model(*model, callback);
            std::cout << "Object " << id << " is referenced in entries:\n";
            if (result.empty()) {
                std::cout << "none\n";
                return;
            }
            int position = 1;
            for (const auto& reference: result) {
                std::cout << position << ". Reference found in element " << reference << std::endl;
                std::optional<std::pair<std::string, arxml::model::INamedAutosarElement&>> root;
                arxml::helpers::RootElementFinder root_finder(root, reference);
                arxml::dfs::traverse_model(*model, root_finder);
                std::cout << "Root element: " << root.value().first << std::endl;
                arxml::printer::TreePrinter::stdout_dump(root.value().second);
                std::cout << std::endl;
                ++position;
            }
        }
    }

    void FinderSubProgram::execute(const std::vector<std::string>& args) {
        if (args[0] != getName()) {
            return;
        }
        if (args.size() == 1 or (args.size() > 1 and args[1] == "help")) {
            std::cerr << help() << std::endl;
            return;
        }
        if (args.size() < 5) {
            throw std::logic_error("Invalid number of the arguments! Expected mode and path or ask for the help.");
        }
        std::string project_configuration = args[1];
        std::string path = args[2];
        std::string mode = args[3];
        std::string name = args[4];

        if (mode == "by_tag") {
            find_by_tag(path, name);
        }
        else if (mode == "by_id") {
            find_by_id(path, name);
        }
        else if (mode == "by_ref") {
            find_by_ref(path, name);
        }
    }

    std::string FinderSubProgram::help() {
        std::stringstream ss;
        ss << "ARXML Tool Finder supports three modes:\n";
        ss << "by_tag - lists all elements defined under given tag\n"
           << "by_ref - shows elements with references to the given object\n"
           << "by_id - shows element with given id\n\n"
           << "Command need to be called on the following ways\n";
        ss  << "1 | help\n"
            << "2 | dir MODEL_DIR_NAME [by_tag|by_id] NAME\n"
            << "3 | config CONFIGURATION_FILE_NAME [by_tag|by_id|by_ref] NAME\n\n";
        ss << "Examples:\n./arxml_tool finder dir data/ by_tag SERVICE-INTERFACE\n";
        ss << "./arxml-tool finder config config.yml by_id /apd/ServiceInterfaces/TestService";
        return ss.str();
    }
}