//
// Created by Paweł Jarosz on 27.12.2023.
//

#include "arxml_tool/program_selector.hpp"

#include <iostream>

namespace arxml_tool {

    ProgramSelector::ProgramSelector(int argc, const char* argv[]) {
        for (int it = 0; it < argc; ++it) {
            m_argv.emplace_back(argv[it]);
        }
    }

    int ProgramSelector::execute() {
        if (m_argv.size() == 1 || (m_argv.size() > 2 and m_argv[1] == "help")) {
            std::cout << "Available commands:" << std::endl;
            std::cout << "help - lists of the available tools" << std::endl;
            std::for_each(m_subprograms.begin(), m_subprograms.end(), [](std::unique_ptr<AbstractSubProgram>& program) {
                std::cout << program->getName() << " - " << program->description() << std::endl;
            });
            return EXIT_FAILURE;
        }
        std::vector<std::string> subcommands(++m_argv.begin(), m_argv.end());
        std::for_each(m_subprograms.begin(), m_subprograms.end(), [&](std::unique_ptr<AbstractSubProgram>& program) {
           program->execute(subcommands);
        });
        return EXIT_SUCCESS;
    }

}
