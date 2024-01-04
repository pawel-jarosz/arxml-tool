//
// Created by Paweł Jarosz on 27.12.2023.
//

#include "arxml_tool/program_selector_builder.hpp"

#include "structure_dump_subprogram.hpp"
#include "dump_tree_subprogram.hpp"
#include "finder_subprogram.hpp"

namespace arxml_tool {

    void ProgramSelectorBuilder::build() {
        m_selector.registerSubProgram(std::unique_ptr<AbstractSubProgram>(new DumpTreeSubprogram));
        m_selector.registerSubProgram(std::unique_ptr<AbstractSubProgram>(new FinderSubProgram));
        m_selector.registerSubProgram(std::unique_ptr<AbstractSubProgram>(new StructureDumpSubProgram));
    }

}
