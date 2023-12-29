//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml_tool/subprograms.hpp>

namespace arxml_tool {
    class FinderSubProgram : public AbstractSubProgram {
    public:
        FinderSubProgram()
                : AbstractSubProgram("finder") {

        }

        void execute(const std::vector<std::string>& args) override;
        std::string description() override { return "find elements of the model"; }
        std::string help() override;
    };
}
