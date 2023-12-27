//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml_tool/subprograms.hpp>

namespace arxml_tool {

    class DumpTreeSubprogram : public AbstractSubProgram {
    public:
        DumpTreeSubprogram()
        : AbstractSubProgram("dump-tree") {

        }

        void execute(const std::vector<std::string>& args) override;
        std::string description() override { return "present AUTOSAR XML tree similar like tree command"; }
        std::string help() override;
    };

}
