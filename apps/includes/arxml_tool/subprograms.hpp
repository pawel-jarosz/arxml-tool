//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <string>
#include <vector>

namespace arxml_tool {

    class AbstractSubProgram {
    public:
        explicit AbstractSubProgram(std::string program_name)
        : m_program_name(std::move(program_name)) {}
        virtual ~AbstractSubProgram() = default;

        const std::string& getName() { return m_program_name; }

        virtual void execute(const std::vector<std::string>& args) = 0;
        virtual std::string description() = 0;
        virtual std::string help() = 0;
    private:
        std::string m_program_name;
    };

}
