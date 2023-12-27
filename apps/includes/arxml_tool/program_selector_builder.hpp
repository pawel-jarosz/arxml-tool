//
// Created by Paweł Jarosz on 27.12.2023.
//

#pragma once

#include <arxml_tool/program_selector.hpp>

namespace arxml_tool {

    class ProgramSelectorBuilder {
    public:
        explicit ProgramSelectorBuilder(ProgramSelector& selector)
        : m_selector{selector}
        {}

        void build();
    private:
        ProgramSelector& m_selector;
    };

}
