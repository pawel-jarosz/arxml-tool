//
// Created by Paweł Jarosz on 27.12.2023.
//

#include "dump_tree_subprogram.hpp"

#include <iostream>
#include <sstream>

#include <arxml/utilities/parser_facade.hpp>
#include <arxml/printer.hpp>
#include <arxml/project.hpp>

namespace arxml_tool {

    void DumpTreeSubprogram::execute(const std::vector<std::string>& args) {
        if (args[0] != getName()) {
            return;
        }
        if (args.size() == 1 or (args.size() > 1 and args[0] == "help")) {
            std::cerr << help() << std::endl;
            return;
        }
        if (args.size() < 3) {
            throw std::logic_error("Invalid number of the arguments! Expected mode and path or ask for the help.");
        }
        std::string mode = args[1];
        std::string path = args[2];
        arxml::utilities::DefaultParserFacade parser;
        arxml::project::ModelProject project;
        project.addDirectory(path);
        arxml::project::ModelProject::openModelFromProject(project, parser);
        auto result = parser.getModel();
        arxml::printer::TreePrinter::stdout_dump(*result);
    }

    std::string DumpTreeSubprogram::help() {
        std::stringstream ss;
        ss << "ARXML Tool in this mode dumps AUTOSAR XML file in a format similar"
           << " to the output of the 'tree' command.\n\n"
           << "Command expects to be called on the following ways:\n";
        ss  << "1 | help\n"
            << "2 | dir MODEL_DIR_NAME\n"
            << "3 | config CONFIGURATION_FILE_NAME\n\n";
        ss << "Example:\n./arxml_tool dump-tree dir data/";
        return ss.str();
    }

}
