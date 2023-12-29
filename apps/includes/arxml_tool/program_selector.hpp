//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

#include <arxml_tool/subprograms.hpp>

namespace arxml_tool {

    class ProgramSelector {
    public:
        ProgramSelector(int argc, const char* argv[]);
        void registerSubProgram(std::unique_ptr<AbstractSubProgram> subprogram) {
            m_subprograms.emplace_back(std::move(subprogram));
        }
        int execute();
    private:
        std::vector<std::string> m_argv;
        std::vector<std::unique_ptr<AbstractSubProgram>> m_subprograms;
    };

}
