//
// Created by Paweł Jarosz on 29.12.2023.
//

#pragma once

#include <arxml_tool/subprograms.hpp>

namespace arxml_tool {

    class StructureDumpSubProgram : public AbstractSubProgram {
    public:
        StructureDumpSubProgram()
                : AbstractSubProgram("structure_dump") {

        }

        void execute(const std::vector<std::string>& args) override;
        std::string description() override { return "dump model structure"; }
        std::string help() override;
    };

}
